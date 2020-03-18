#include "Program.h"
#include "../parser.h"
#include <fstream>

//print the program, dont include the library
std::string Program::toJSON() {
    std::stringstream s;
    s << "{";

    // definitions
    s << "\"library-definitions\":[";
    std::string sep = "";
    for(auto d: library) {
        s << sep;
        s << "{" << d->toJSON() << "}";
        sep = ",";
    }
    s << "]";

    s << ",";

    s << "\"user-definitions\":[";
    sep = "";
    for(auto d: table->defines) {
        s << sep;
        s << "{" << d->toJSON() << "}";
        sep = ",";
    }
    s << "]";

    s << ",";

    // statements
    s << "\"statements\":[";
    sep = "";
    for(auto state: statements) {
        s << sep;
        s << "{" << state->toJSON() << "}";
        sep = ",";
    }
    s<< "]";

    s << "}";

    return s.str();
}

//read in the library file and append to the vector the definitons, everything else is trash
void Program::readLibrary(std::string file) {

    //parse the library file
    std::ifstream in(file);
    Parser p(in);
    p.parse();

    //read in the new definitions, dont care about anything else
    std::vector<Define*> newDefs = p.getProgram()->table->defines;

    //append these defines to the end of the library
    for(auto d: newDefs) {
        this->library.push_back(d);
    }
}


//apply reduction in normal order to all expressions
void Program::betaReduceNormalOrder() {
    for(auto e: statements) {
        e->betaReduceNormalOrder();
    }
}

//apply reduction in call by value to all expressions
void Program::betaReduceCallByValue() {
    for(auto e: statements) {
        e->betaReduceCallByValue();
    }
}