
#include <iostream>
#include <cstdlib>
#include "parser.h"

void Parser::syntax_error()
{
    std::cout << "SYNTAX ERROR !!!" << std::endl;
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type) syntax_error();
    return t;
}

//call parse input
void Parser::parse() {
    parse_input();
    expect(END_OF_FILE);
}

//parser functions
//input -> lines
void Parser::parse_input() {
    parse_lines();
}
//lines -> line
//lines -> line lines
void Parser::parse_lines() {
    parse_line();
    //a line either starts with an named expr or a term
    //a named expr starst with an AT
    //a term starts with an ID, AT, LPAREN, LCURLY, or LBRACK
    Token t = lexer.Peek();
    if(t.token_type == ID ||
       t.token_type == AT ||
       t.token_type == LPAREN ||
       t.token_type == LCURLY ||
       t.token_type == LBRACK) {
        parse_lines();
    }
}
//line -> expr ;
void Parser::parse_line() {
    parse_expr();
    expect(SEMICOLON);
}
//expr -> named
//expr -> term
void Parser::parse_expr() {
    //named starts with an AT
    //term stars with an ID, AT, LPAREN, LCURLY, or LBRACK
    //need to distinguish, will need three tokens
    Token t1 = lexer.Peek();
    if(t1.token_type == AT) {
        //consume t1 and get 2 more
        t1 = lexer.GetToken();
        Token t2 = lexer.GetToken();
        Token t3 = lexer.GetToken();
        //put back in order
        lexer.UngetToken(t3);
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        //if t3 is EQUALS, we have a named expr
        if(t3.token_type == EQUALS) {
            parse_named();
        }
        //not a named, parse the term
        else {
            parse_term();
        }
    }
    else if(t1.token_type == ID ||
            t1.token_type == AT ||
            t1.token_type == LPAREN ||
            t1.token_type == LCURLY ||
            t1.token_type == LBRACK) {
        parse_term();
    }
    else {
        //nothing else can be expr, error
        syntax_error();
    }
}
//var -> ID
void Parser::parse_var() {
    expect(ID);
}
//name -> AT var
void Parser::parse_name() {
    expect(AT);
    parse_var();
}
//named -> name EQUALS term
//named -> name EQUALS name
void Parser::parse_named() {
    parse_name();
    expect(EQUALS);
    //name starts with an AT
    //term stars with an ID, AT, LPAREN, LCURLY, or LBRACK
    //need to distinguish, will need three tokens
    Token t1 = lexer.Peek();
    if(t1.token_type == AT) {
        //consume t1 and get 2 more
        t1 = lexer.GetToken();
        Token t2 = lexer.GetToken();
        Token t3 = lexer.GetToken();
        //put back in order
        lexer.UngetToken(t3);
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        //if t3 is SEMICOLON, we have name
        if(t3.token_type == EQUALS) {
            parse_name();
        }
        //not a name, parse the term
        else {
            parse_term();
        }
    }
    else if(t1.token_type == ID ||
            t1.token_type == AT ||
            t1.token_type == LPAREN ||
            t1.token_type == LCURLY ||
            t1.token_type == LBRACK) {
        parse_term();
    }
    else {
        //nothing else can be named, error
        syntax_error();
    }


}
//atom -> var
//atom -> name
void Parser::parse_atom() {
    Token t = lexer.Peek();
    if(t.token_type == ID) {
        parse_var();
    }
    else if (t.token_type == AT) {
        parse_name();
    }
    else {
        syntax_error();
    }
}
//1. term -> atom
//2. term -> LPAREN term RPAREN
//3. term -> LCURLY LAMBDA var DOT term RCURLY
//4. term -> LBRACK term term RBRACK
void Parser::parse_term() {

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
}