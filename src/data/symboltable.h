
#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <vector>

#include "Expression.h"
#include "Application.h"
#include "Abstraction.h"
#include "Term.h"
#include "Atom.h"
#include "Variable.h"
#include "Name.h"

class SymbolTable {
public:
    std::vector<Expression*> expressions;
    std::vector<Application*> applications;
    std::vector<Abstraction*> abstractions;
    std::vector<Term*> terms;
    std::vector<Atom*> atoms;
    std::vector<Variable*> variables;
    std::vector<Name*> names;
};


#endif
