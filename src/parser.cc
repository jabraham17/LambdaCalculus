
#include <iostream>
#include <cstdlib>
#include "parser.h"

void Parser::syntax_error()
{
    std::cout << "Syntax Error on line " << preprocessor.getLineNumber() << std::endl;
    exit(1);
}
void Parser::syntax_error(std::string message)
{
    std::cout << "Syntax Error on line " << preprocessor.getLineNumber() << std::endl;
    std::cout << "Error: " << message << std::endl;
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = preprocessor.getToken();
    if(t.token_type != expected_type) {

        std::string message = "Token of type '" +
                                Token::types[t.token_type] +
                                "' instead of '" +
                                Token::types[expected_type] +
                                "'";
        syntax_error(message);
    }
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
//line -> term SEMICOLON
//line -> define SEMICOLON
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
        Define* def = parse_define();
        //add define to the list
        expect(SEMICOLON);

        program->addUserDefine(def);

    }
    else if(t1.token_type == ID ||
            t1.token_type == AT ||
            t1.token_type == LPAREN ||
            t1.token_type == LCURLY ||
            t1.token_type == LBRACK) {

        Term* t = parse_term();
        expect(SEMICOLON);

        //add the expression to the list of statements
        program->addStatement(t);

    }
    else {
        //nothing else can be expr, error
        syntax_error();
    }
}
//define -> name EQUALS term
Define* Parser::parse_define() {
    Name* name = parse_name();
    expect(EQUALS);
    Term* t = parse_term();

    Define* d = program->getSymbolTable()->createDefine(name, t);
    return d;
}
//var -> ID
Variable* Parser::parse_variable() {
    Token t = expect(ID);
    return program->getSymbolTable()->createVariable(t.lexeme);
}
//name -> AT ID
Name* Parser::parse_name() {
    expect(AT);
    Token t = expect(ID);

    return program->getSymbolTable()->createName(t.lexeme);
}

//1. term -> variable
//2. term -> name
//3. term -> LPAREN term RPAREN
//4. term -> LCURLY LAMBDA variable DOT term RCURLY
//5. term -> LBRACK term term RBRACK
Term* Parser::parse_term() {

    Token t = preprocessor.peek();

    //1
    if(t.token_type == ID) {
        Variable* var = parse_variable();
        return program->getSymbolTable()->createTerm(var);
    }
    //2
    else if(t.token_type == AT) {
        Name* name = parse_name();
        return program->getSymbolTable()->createTerm(name);
    }
    //3
    else if(t.token_type == LPAREN) {
        expect(LPAREN);
        Term* term = parse_term();
        term->addParen();
        expect(RPAREN);
        return term;
    }
    //4
    else if(t.token_type == LCURLY) {
        expect(LCURLY);
        expect(LAMBDA);
        Variable* var = parse_variable();
        var->makeParameter(ParameterID::ID());
        expect(DOT);
        Term* term = parse_term();
        expect(RCURLY);
        return program->getSymbolTable()->createTerm(var, term);
    }
    //5
    else if(t.token_type == LBRACK) {
        expect(LBRACK);
        Term* a = parse_term();
        Term* b = parse_term();
        expect(RBRACK);
        return program->getSymbolTable()->createTerm(a, b);
    }
    else {
        syntax_error();
    }
    return NULL;
}