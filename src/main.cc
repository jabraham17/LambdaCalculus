
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "parser.h"
#include "preprocessor.h"

int main (int argc, char* argv[])
{
    //if a debug input var is set, redirect cin to it
    if(const char* redirected_stdin = std::getenv("__DEBUG_INPUT__")) {
        freopen(redirected_stdin, "r", stdin);
    }

    //Parser p;
    //p.parse();



    Preprocessor pre;
    Token t = pre.Peek();
    while(t.token_type != END_OF_FILE and t.token_type != ERROR) {
        t = pre.GetToken();
        t.Print();
    }

    return 0;
}

