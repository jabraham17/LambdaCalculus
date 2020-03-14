

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
    int getLineNumber() {return lexer.getLineNumber();};

private:
    std::vector<Token> tokens;
    LexicalAnalyzer lexer;

    //reads a full line from the lexer
    void readline(std::vector<Token>&);
    //preprocess a line of input
    void preprocess();
    void applyAbstraction(std::vector<Token>&);
    void applyApplication(std::vector<Token>&);
    std::vector<Token> applyApplicationToTerm(std::vector<Token>);
    void identifyAndApply(std::vector<Token>&);
    int identifyTerm(std::vector<Token>, int);
    int identifyParen(std::vector<Token>, int);
    int identifyBody(std::vector<Token>, int, TokenType, TokenType);
    int identifyAbstractionBody(std::vector<Token>, int);
    std::vector<Token> sliceVector(std::vector<Token>, int, int);
    void replaceRange(std::vector<Token>&, int, int, std::vector<Token>);

};


#endif
