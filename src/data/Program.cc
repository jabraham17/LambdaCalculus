#include "Program.h"
#include "../parser.h"
#include <fstream>


Program::Program(): statements(), table(new SymbolTable()), library(), userLibrary(), allDefines() {}
Program::~Program() {
    delete(table);
}

std::vector<Term*>& Program::getStatements() {return statements;}
SymbolTable*& Program::getSymbolTable() {return table;}
std::vector<Define*>& Program::getLibraryDefines() {return library;}
std::vector<Define*>& Program::getUserLibraryDefines() {return library;}
std::vector<Define*>& Program::getAllDefines() {return allDefines;}

void Program::addStatement(Term* t) {statements.push_back(t);}

void Program::addDefine(Define* d) {
    this->library.push_back(d);
    this->allDefines.push_back(d);
}
void Program::addDefines(std::vector<Define*> ds) {
    for(auto d: ds) {
        addDefine(d);
    }
}
void Program::addUserDefine(Define* d) {
    this->userLibrary.push_back(d);
    this->allDefines.push_back(d);
}
void Program::addUserDefines(std::vector<Define*> ds) {
    for(auto d: ds) {
        addUserDefine(d);
    }
}


std::ostream& operator<<(std::ostream& out, const Program& p) {
    std::string sep;
    for(auto statement: p.statements) {
        out << sep << *statement;
        sep = '\n';
    }
    return out;
}
std::string Program::toJSON() {
    std::stringstream s;
    s << "{";

    // definitions
    s << "\"library-definitions\":[";
    std::string sep = "";
    for(auto d: getLibraryDefines()) {
        s << sep << "{" << d->toJSON() << "}";
        sep = ",";
    }
    s << "],";


    s << "\"user-definitions\":[";
    sep = "";
    for(auto d: getSymbolTable()->defines) {
        s << sep <<"{" << d->toJSON() << "}";
        sep = ",";
    }
    s << "],";

    // statements
    s << "\"statements\":[";
    sep = "";
    for(auto state: getStatements()) {
        s << sep << "{" << state->toJSON() << "}";
        sep = ",";
    }
    s<< "]";
    s << "}";

    return s.str();
}


//read in the library file and append to the vector the definitons, everything else is trash
std::vector<Define*> readLibrary(std::string file) {

    //parse the library file
    std::ifstream in(file);
    Parser p(in);
    p.parse();

    Program* lib = p.getProgram();


    //read in the new definitions, dont care about anything else
    std::vector<Define*> newDefs = lib->getAllDefines();
    return newDefs;
}

/*

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
}*/