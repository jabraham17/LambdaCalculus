
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>
#include <istream>
#include <iostream>
#include <ostream>


enum TokenType { END_OF_FILE=0,
               DOT,
               LPAREN,
               RPAREN,
               LAMBDA,
               ID,
               SEMICOLON,
               EQUALS,
               AT,
               LBRACK,
               RBRACK,
               LCURLY,
               RCURLY,
               ERROR
             };

class Token {
    public:

    std::string lexeme;
    TokenType token_type;
    int line_no;

    Token(std::string lexeme, TokenType token_type, int line_no): lexeme(lexeme), token_type(token_type), line_no(line_no) {}
    Token(): lexeme(""), token_type(ERROR), line_no(-1) {}

    static const std::string types[];
    friend std::ostream& operator<<(std::ostream& out, const Token& t) {
        out << "{" << Token::types[t.token_type] << ",";
        if(!t.lexeme.empty()) out << t.lexeme << ",";
        out << t.line_no << "}";
        return out;
    }
};

class Lexer {
    public:
    Token getToken();
    Token peek();
    TokenType ungetToken(Token);
    int getLineNumber() {return line_number;};
    explicit Lexer(std::istream& input): input(input), line_number(1), tokens(), input_buffer() {}
    Lexer(): input(std::cin), line_number(1), tokens(), input_buffer() {}

    private:
    std::istream& input;
    int line_number;
    std::vector<Token> tokens;
    std::vector<char> input_buffer;

    //input buffer code
    bool endOfInput();
    char peekChar();
    void ungetChar(char);
    char getChar();


    void skipWhiteSpace();
    Token scanId();
};

#endif
