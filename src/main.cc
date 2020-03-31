
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
    prog->addDefines(readLibrary("library/boolean.lcl"));
    prog->addDefines(readLibrary("library/arithmetic.lcl"));
    prog->addDefines(readLibrary("library/number.lcl"));
    prog->addDefines(readLibrary("library/pair.lcl"));
    prog->addDefines(readLibrary("library/test.lcl"));
    prog->addDefines(readLibrary("library/control.lcl"));


    //reduce in normal order
    //prog->determineBinding();
    //prog->betaReduceNormalOrder();
    //prog->checkForDefines();

    std::cout << *prog << std::endl;
    prog->betaReduceNormalOrder();
    std::cout << *prog << std::endl;
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

