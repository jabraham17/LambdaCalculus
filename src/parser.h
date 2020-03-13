#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include <vector>

class Parser {
    private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();

    //parser functions
    //input -> lines
    void parse_input();
    //lines -> line
    //lines -> line lines
    void parse_lines();
    //line -> expr SEMICOLON
    void parse_line();
    //expr -> named
    //expr -> term
    void parse_expr();
    //var -> ID
    void parse_var();
    //name -> AT var
    void parse_name();
    //named -> name EQUALS term
    //named -> name EQUALS name
    void parse_named();
    //atom -> var
    //atom -> name
    void parse_atom();
    //term -> atom
    //term -> LPAREN term RPAREN
    //term -> LCURLY LAMBDA var DOT term RCURLY
    // term -> LBRACK term term RBRACK
    void parse_term();


    //public interface
    public:
    void parse();
};

#endif

