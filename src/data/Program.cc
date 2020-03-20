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

    Program* lib = p.getProgram();

    //before we read the defintions, we need to determine bindings
    lib->determineBinding();


    //read in the new definitions, dont care about anything else
    std::vector<Define*> newDefs = lib->table->defines;

    //append these defines to the end of the library
    for(auto d: newDefs) {
        this->library.push_back(d);
    }
}

//put all defines in a list, user defined first
std::vector<Define*> Program::allDefines() {
    std::vector<Define*> allDefs;
    for(auto d: this->table->defines) allDefs.push_back(d);
    for(auto d: this->library) allDefs.push_back(d);
    return allDefs;
}

//apply reduction in normal order to all expressions
void Program::betaReduceNormalOrder() {

    auto defs = allDefines();

    for(auto e: statements) {
        e->betaReduceNormalOrder(defs);
    }
}

//apply reduction in call by value to all expressions
void Program::betaReduceCallByValue() {
    for(auto e: statements) {
        e->betaReduceCallByValue();
    }
}

//determine all abstractions bindings
void Program::determineBinding() {
    for(auto a: this->table->abstractions) {
        a->determineBinding();
    }
}

//check if this statement can be expressed as a define
void Program::checkForDefines() {
    auto defs = allDefines();

    for(auto e: statements) {
        for(auto d: defs) {
            if(*(e->term) == *(d->getExpression()->term)) {
                e->term = new Term(-1, new Atom(-1, d->getName()), true);
            }
        }
    }
}