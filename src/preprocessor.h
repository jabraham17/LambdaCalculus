

#ifndef __PREPROCESSOR_H_
#define __PREPROCESSOR_H_

#include <vector>
#include "lexer.h"

//works on the token stream and inserts tokens as needed
class Preprocessor {

public:
    Token GetToken();
    Token Peek();
    TokenType UngetToken(Token);

private:
    std::vector<Token> tokens;
    LexicalAnalyzer lexer;

    //reads a full line from the lexer
    void readline(std::vector<Token>&);
    //preprocess a line of input
    void preprocess();
    void applyAbstraction(std::vector<Token>&);
    void applyApplication(std::vector<Token>&, int, int);
    void applyApplication(std::vector<Token>&);
    int identifyParen(std::vector<Token>, int);
    void identifyTerm(std::vector<Token>, int, int&, int&);
};


#endif
