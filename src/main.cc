
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "parser.h"
#include "data/Program.h"
#include "json_helper.h"
#include "preprocessor.h"
#include <iostream>

int main (int argc, char* argv[])
{
    //if a debug input var is set, redirect cin to it
    if(const char* redirected_stdin = std::getenv("__DEBUG_INPUT__")) {
        freopen(redirected_stdin, "r", stdin);
    }

    Parser parser;
    Program* prog = NULL;
    parser.parse();
    prog = parser.getProgram();
    std::cout << *prog << std::endl;

    //std::cout << prog->createASTNode() << std::endl;
    std::cout << JSON::pretty(prog->createASTNode()) << std::endl;

/*
    Preprocessor pre;
    Token t = pre.Peek();
    while(t.token_type != END_OF_FILE and t.token_type != ERROR) {
        t = pre.GetToken();
        t.Print();
    }*/

    return 0;
}

