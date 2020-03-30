#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <string>
#include <ostream>
#include <sstream>

#include "symboltable.h"
#include "Define.h"

class Program {
private:
    std::vector<Term*> statements;
    SymbolTable* table;
    std::vector<Define*> library;
    std::vector<Define*> userLibrary;
    std::vector<Define*> allDefines;

public:
    Program();
    ~Program();

    std::vector<Term*>& getStatements();
    SymbolTable*& getSymbolTable();
    std::vector<Define*>& getLibraryDefines();
    std::vector<Define*>& getUserLibraryDefines();
    std::vector<Define*>& getAllDefines();

    void addStatement(Term*);
    void addDefine(Define*);
    void addDefines(std::vector<Define*>);
    void addUserDefine(Define*);
    void addUserDefines(std::vector<Define*>);

    friend std::ostream& operator<<(std::ostream&, const Program&);
    std::string toJSON();


    //TODO
    /*std::vector<Define*> allDefines();

    //determine all abstarctions bindings
    void determineBinding();

    //apply reduction in normal order to all expressions
    void betaReduceNormalOrder();

    //apply reduction in call by value to all expressions
    void betaReduceCallByValue();

    void readLibrary(std::string);

    //check if this statement can be expressed as a define
    void checkForDefines();*/

};

std::vector<Define*> readLibrary(std::string);

#endif
