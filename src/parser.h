#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include <vector>
#include "data/Program.h"
#include "preprocessor.h"

class Parser {

    //public interface
    public:

    Parser(): preprocessor(), program(new Program()) {}
    explicit Parser(std::istream& in): preprocessor(in), program(new Program()) {}
    explicit Parser(Lexer lexer): preprocessor(lexer), program(new Program()) {}
    explicit Parser(Preprocessor preprocessor): preprocessor(preprocessor), program(new Program()) {}

    //methods to compile the program
    void parse();
    Program* getProgram() {return program;}

    private:
    Preprocessor preprocessor;
    Program* program;

    void syntax_error();
    void syntax_error(std::string);
    Token expect(TokenType expected_type);
    Token peek();

    //parser functions
    //input -> lines
    void parse_input();
    //lines -> line
    //lines -> line lines
    void parse_lines();
    //line -> expr SEMICOLON
    //line -> define SEMICOLON
    void parse_line();
    //expr -> term
    Expression* parse_expr();
    //define -> name EQUALS expr
    Define* parse_define();
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

};

#endif

