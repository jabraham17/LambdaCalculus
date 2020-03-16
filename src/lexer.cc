
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"

using namespace std;

// Lexer modified for FIRST & FOLLOW project

const std::string types[] = { "END_OF_FILE",
                      "DOT",
                      "LPAREN",
                      "RPAREN",
                      "LAMBDA",
                      "ID",
                      "SEMICOLON",
                      "EQUALS",
                      "AT",
                      "LBRACK",
                      "RBRACK",
                      "LCURLY",
                      "RCURLY",
                      "ERROR"
                    };

void Lexer::skipWhiteSpace()
{
    //get a new char
    char c = getChar();

    //while not eof and c is whitespace
    while(!endOfInput() && isspace(c)) {
        //if c is \n, increase line number
        if(c == '\n') c++;
        c = getChar();
    }
    //if we got one too many, put it back
    if(!endOfInput()) ungetChar(c);

}

Token Lexer::scanId()
{
    char c = getChar();

    //token to return
    Token t;

    //if its a char
    if(isalpha(c)) {
        std::string lexeme = "";
        while(!endOfInput() && isalnum(c)) {
            lexeme += c;
            c = getChar();
        }
        t.lexeme = lexeme;
        t.line_no = line_number;
        t.token_type = ID;
    }
    //not a char, put it back
    if(!endOfInput()) ungetChar(c);
    return t;
}

TokenType Lexer::ungetToken(Token t)
{
    tokens.push_back(t);;
    return t.token_type;
}

Token Lexer::peek() {
    Token t = getToken();
    ungetToken(t);
    return t;
}

Token Lexer::getToken()
{
    //if we have tokens, get them,
    if(!tokens.empty()) {
        Token t = tokens.back();
        tokens.pop_back();
        return t;
    }

    //skip all spaces
    skipWhiteSpace();
    Token t;
    t.line_no = line_number;
    t.token_type = END_OF_FILE;

    //if not eof, get char, otherwise return eof
    char c;
    if(!endOfInput()) c = getChar();
    else return t;
    t.lexeme = c;
    
    switch (c) {
        case '.': t.token_type = DOT; return t;
        case '(': t.token_type = LPAREN; return t;
        case ')': t.token_type = RPAREN; return t;
        case '[': t.token_type = LBRACK; return t;
        case ']': t.token_type = RBRACK; return t;
        case '{': t.token_type = LCURLY; return t;
        case '}': t.token_type = RCURLY; return t;
        case '$': t.token_type = LAMBDA; return t;
        case ';': t.token_type = SEMICOLON; return t;
        case '=': t.token_type = EQUALS; return t;
        case '@': t.token_type = AT; return t;
        default:
            //if alpha, get the id
            if(isalpha(c)) {
                ungetChar(c);
                return scanId();
            }
            else if(endOfInput()) t.token_type = END_OF_FILE;
            else t.token_type = ERROR;
            return t;
    }
}


bool Lexer::endOfInput()
{
    if(!input_buffer.empty()) return false;
    else return input.eof();
}

char Lexer::peekChar()
{
    char c = getChar();
    ungetChar(c);
    return c;
}

void Lexer::ungetChar(char c)
{
    if (c != EOF) input_buffer.push_back(c);
}

char Lexer::getChar()
{
    char c;
    if(!input_buffer.empty()) {
        c = input_buffer.back();
        input_buffer.pop_back();
    }
    else input.get(c);

    return c;
}
