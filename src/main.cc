
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
    parser.parse();

    Program* prog = NULL;
    prog = parser.getProgram();

    //read the library
    prog->readLibrary("library/boolean");
    prog->readLibrary("library/arithmetic");
    prog->readLibrary("library/number");
    prog->readLibrary("library/pair");
    prog->readLibrary("library/test");
    prog->readLibrary("library/control");


    //print the statements
    std::cout << *prog << std::endl;

    prog->determineBinding();
    prog->betaReduceNormalOrder();

    std::cout << std::endl << *prog << std::endl;


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

