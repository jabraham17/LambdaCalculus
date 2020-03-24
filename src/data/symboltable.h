
#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <vector>

#include "Expression.h"
#include "Define.h"
#include "Application.h"
#include "Abstraction.h"
#include "Term.h"
#include "Atom.h"
#include "Variable.h"
#include "Name.h"

//TODO: add fucntions for adding stuff with global ids so every element is marked as 'unique' and no duplicates exist

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
