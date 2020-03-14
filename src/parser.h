#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include <vector>
#include "data/Program.h"
#include "preprocessor.h"

class Parser {
    private:
    Preprocessor lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();

    //parser functions
    //input -> lines
    void parse_input(Program*);
    //lines -> line
    //lines -> line lines
    void parse_lines(Program*);
    //line -> expr SEMICOLON
    Expression* parse_line();
    //expr -> term
    //expr -> name EQUALS term
    Expression* parse_expr();
    //var -> ID
    Variable* parse_var();
    //name -> AT ID
    Name* parse_name();
    //atom -> var
    //atom -> name
    Atom* parse_atom();
    //term -> atom
    //term -> LPAREN term RPAREN
    //term -> LCURLY LAMBDA var DOT term RCURLY
    //term -> LBRACK term term RBRACK
    Term* parse_term();


    //public interface
    public:
    Program* parse();
};

#endif

