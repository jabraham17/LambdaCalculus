
#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <vector>

#include "Define.h"
#include "Term.h"
#include "Variable.h"
#include "Name.h"

class SymbolTable {
    
    public:

    SymbolTable(): defines(), expressions(), applications(), abstractions(), terms(), atoms(), variables(), names() {}

    //check if a name already exists, if it does return the one that already exists
    Name* exists(std::string);


    //create blanks elements
    Define* createDefine(Name*, Term*);
    Term* createTerm(Variable*);
    Term* createTerm(Name*);
    Term* createTerm(Variable*, Term*);
    Term* createTerm(Term*, Term*);
    Variable* createVariable(std::string);
    Name* createName(std::string);


    std::vector<Define*> defines;
    std::vector<Variable*> variables;
    std::vector<Name*> names;
    std::vector<Term*> terms;
};


#endif
