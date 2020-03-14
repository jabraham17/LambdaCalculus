
#include <iostream>
#include <cstdlib>
#include "parser.h"

void Parser::syntax_error()
{
    std::cout << "Syntax Error on line " << lexer.getLineNumber() << std::endl;
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type) syntax_error();
    return t;
}

//call parse input
Program* Parser::parse() {
    Program* p = new Program();
    parse_input(p);
    expect(END_OF_FILE);
    return p;
}

//parser functions
//input -> lines
void Parser::parse_input(Program* p) {
    parse_lines(p);
}
//lines -> line
//lines -> line lines
void Parser::parse_lines(Program* p) {
    Expression* expr = parse_line();
    //add the expression to the program
    p->statements.push_back(expr);

    // TODO: add the expression and all its compoennts to the symbol table

    //a line either starts with an named expr or a term
    //a named expr starts with an AT
    //a term starts with an ID, AT, LPAREN, LCURLY, or LBRACK
    Token t = lexer.Peek();
    if(t.token_type == ID ||
       t.token_type == AT ||
       t.token_type == LPAREN ||
       t.token_type == LCURLY ||
       t.token_type == LBRACK) {
        parse_lines(p);
    }
}
//line -> expr ;
Expression* Parser::parse_line() {
    Expression* expr = parse_expr();
    expect(SEMICOLON);
    return expr;
}
//expr -> name EQUALS term
//expr -> term
Expression* Parser::parse_expr() {
    //name starts with an AT
    //term stars with an ID, AT, LPAREN, LCURLY, or LBRACK
    //need to distinguish, will need three tokens

    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    Token t3 = lexer.GetToken();
    //put back in order
    lexer.UngetToken(t3);
    lexer.UngetToken(t2);
    lexer.UngetToken(t1);

    //if t3 is EQUALS, we have a named expr
    if(t1.token_type == AT && t3.token_type == EQUALS) {
        Name* name = parse_name();
        expect(EQUALS);
        Term* term = parse_term();
        Expression* expr = new Expression(name, term);
        //now we know name's expression
        name->expression = expr;
        return expr;
    }
    else if(t1.token_type == ID ||
            t1.token_type == AT ||
            t1.token_type == LPAREN ||
            t1.token_type == LCURLY ||
            t1.token_type == LBRACK) {
        Term* term = parse_term();
        return new Expression(term);
    }
    else {
        //nothing else can be expr, error
        syntax_error();
    }
    return NULL;
}
//var -> ID
Variable* Parser::parse_var() {
    Token t = expect(ID);
    return new Variable(t.lexeme);
}
//name -> AT ID
Name* Parser::parse_name() {
    expect(AT);
    Token t = expect(ID);
    return new Name(t.lexeme, NULL);
}

//atom -> var
//atom -> name
Atom* Parser::parse_atom() {
    Token t = lexer.Peek();
    if(t.token_type == ID) {
        Variable* newVar  = parse_var();
        return new Atom(newVar);
    }
    else if (t.token_type == AT) {
        Name* newName = parse_name();
        return new Atom(newName);
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
/*void Parser::parse_term() {

    Token t = lexer.Peek();

    //1
    if(t.token_type == ID ||
       t.token_type == AT) {
        parse_atom();
    }
    //2
    else if(t.token_type == LPAREN) {
        expect(LPAREN);
        parse_term();
        expect(RPAREN);
    }
    //3
    else if(t.token_type == LCURLY) {
        expect(LCURLY);
        expect(LAMBDA);
        parse_var();
        expect(DOT);
        parse_term();
        expect(RCURLY);
    }
    //4
    else if(t.token_type == LBRACK) {
        expect(LBRACK);
        parse_term();
        parse_term();
        expect(RBRACK);
    }
    else {
        syntax_error();
    }
}*/

//1. term -> atom
//2. term -> LPAREN term RPAREN
//3. term -> LCURLY LAMBDA var DOT term RCURLY
//4. term -> LBRACK term term RBRACK
Term* Parser::parse_term() {

    Token t = lexer.Peek();

    //1
    if(t.token_type == ID ||
       t.token_type == AT) {
        Atom* atom = parse_atom();
        return new Term(atom);
    }
    //2
    else if(t.token_type == LPAREN) {
        expect(LPAREN);
        Term* term = parse_term();
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
        Abstraction* abs = new Abstraction(var, term);
        return new Term(abs);
    }
    //4
    else if(t.token_type == LBRACK) {
        expect(LBRACK);
        Term* a = parse_term();
        Term* b = parse_term();
        expect(RBRACK);
        Application* app = new Application(a, b);
        return new Term(app);
    }
    else {
        syntax_error();
    }
    return NULL;
}