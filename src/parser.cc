
#include <iostream>
#include <cstdlib>
#include "parser.h"

void Parser::syntax_error()
{
    std::cout << "Syntax Error on line " << preprocessor.getLineNumber() << std::endl;
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = preprocessor.getToken();
    if(t.token_type != expected_type) syntax_error();
    return t;
}


//call parse input
void Parser::parse() {
    parse_input();
}

//parser functions
//input -> lines
void Parser::parse_input() {
    parse_lines();
    expect(END_OF_FILE);
}
//lines -> line
//lines -> line lines
void Parser::parse_lines() {
    parse_line();

    //a line either starts with an named expr or a term
    //a named expr starts with an AT
    //a term starts with an ID, AT, LPAREN, LCURLY, or LBRACK
    Token t = preprocessor.peek();
    if(t.token_type == ID ||
       t.token_type == AT ||
       t.token_type == LPAREN ||
       t.token_type == LCURLY ||
       t.token_type == LBRACK) {
        parse_lines();
    }
}
//line -> expr ;
//line -> define ;
void Parser::parse_line() {

    //define starts with an AT
    //expr stars with an ID, AT, LPAREN, LCURLY, or LBRACK
    //need to distinguish, will need three tokens

    Token t1 = preprocessor.getToken();
    Token t2 = preprocessor.getToken();
    Token t3 = preprocessor.getToken();
    //put back in order
    preprocessor.ungetToken(t3);
    preprocessor.ungetToken(t2);
    preprocessor.ungetToken(t1);

    //if t3 is EQUALS, we have a define
    if(t1.token_type == AT && t3.token_type == EQUALS) {
        parse_define();
        //define is already stored in symbol table, dont need to do anything further
        expect(SEMICOLON);

    }
    else if(t1.token_type == ID ||
            t1.token_type == AT ||
            t1.token_type == LPAREN ||
            t1.token_type == LCURLY ||
            t1.token_type == LBRACK) {

        Expression* expr = parse_expr();
        expect(SEMICOLON);

        //add the expression to the list of statements
        program->statements.push_back(expr);

    }
    else {
        //nothing else can be expr, error
        syntax_error();
    }
}
//expr -> term
Expression* Parser::parse_expr() {
    Term* term = parse_term();
    Expression* expr = program->table->createExpression(term);
    return expr;
}
//define -> name EQUALS expr
Define* Parser::parse_define() {
    Name* name = parse_name();
    expect(EQUALS);
    Expression* expr = parse_expr();

    Define* d = program->table->createDefine(name, expr);
    return d;
}
//var -> ID
Variable* Parser::parse_var() {
    Token t = expect(ID);
    return program->table->createVariable(t.lexeme);
}
//name -> AT ID
Name* Parser::parse_name() {
    expect(AT);
    Token t = expect(ID);

    return program->table->createName(t.lexeme);
}

//atom -> var
//atom -> name
Atom* Parser::parse_atom() {
    Token t = preprocessor.peek();
    if(t.token_type == ID) {
        Variable* newVar  = parse_var();
        return program->table->createAtom(newVar);
    }
    else if (t.token_type == AT) {
        Name* newName = parse_name();
        return program->table->createAtom(newName);
    }
    else {
        syntax_error();
    }
    return NULL;
}

//1. term -> atom
//2. term -> LPAREN term RPAREN
//3. term -> LCURLY LAMBDA var DOT term RCURLY
//4. term -> LBRACK term term RBRACK
Term* Parser::parse_term() {

    Token t = preprocessor.peek();

    //1
    if(t.token_type == ID ||
       t.token_type == AT) {
        Atom* atom = parse_atom();
        return program->table->createTerm(atom, false);
    }
    //2
    else if(t.token_type == LPAREN) {
        expect(LPAREN);
        Term* term = parse_term();
        term->addParen();
        expect(RPAREN);
        return term;
    }
    //3
    else if(t.token_type == LCURLY) {
        expect(LCURLY);
        expect(LAMBDA);
        Variable* var = parse_var();
        expect(DOT);
        Term* term = parse_term();
        expect(RCURLY);
        Abstraction* abs = program->table->createAbstraction(var, term);
        return program->table->createTerm(abs, false);
    }
    //4
    else if(t.token_type == LBRACK) {
        expect(LBRACK);
        Term* a = parse_term();
        Term* b = parse_term();
        expect(RBRACK);
        Application* app = program->table->createApplication(a, b);
        return program->table->createTerm(app, false);
    }
    else {
        syntax_error();
    }
    return NULL;
}