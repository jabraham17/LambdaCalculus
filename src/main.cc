
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "parser.h"
#include "data/Program.h"
#include "json_helper.h"
#include "preprocessor.h"
#include <iostream>

int main()
{
    //if a debug input var is set, redirect cin to it
    if(const char* redirected_stdin = std::getenv("__DEBUG_INPUT__")) {
        freopen(redirected_stdin, "r", stdin);
    }

    Parser parser;
    Program* prog = NULL;
    parser.parse();
    prog = parser.getProgram();

    //read the library
    prog->readLibrary("library/boolean");
    prog->readLibrary("library/arithmetic");
    prog->readLibrary("library/number");
    prog->readLibrary("library/pair");
    prog->readLibrary("library/test");
    prog->readLibrary("library/recursive_helper");


    //print the statements
    std::cout << *prog << std::endl;
    if(prog->statements[0]->term->isBetaRedex()) {
        std::cout << "found redex" << std::endl;
        applyBetaRedex(prog->statements[0]->term);
        std::cout << *prog << std::endl;
    }

    //std::cout << JSON::pretty(prog->toJSON()) << std::endl;

/*
    Preprocessor pre;
    Token t = pre.Peek();
    while(t.token_type != END_OF_FILE and t.token_type != ERROR) {
        t = pre.GetToken();
        t.Print();
    }*/

    return 0;
}

