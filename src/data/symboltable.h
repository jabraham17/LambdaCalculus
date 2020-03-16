
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

//TODO: add fucntions for adding stuff with global ids so every element is marked as 'unique' and no duplicates exist

class SymbolTable {
    
    public:

    SymbolTable(): expressions(), applications(), abstractions(), terms(), atoms(), variables(), names() {}

    //check if a name already exists, if it does return the one that already exists
    Name* exists(std::string);


    //create blanks elements
    Expression* createExpression(Name*, Term*);
    Expression* createExpression(Term*);
    Application* createApplication(Term*, Term*);
    Abstraction* createAbstraction(Variable*, Term*);
    Term* createTerm(Atom*, bool);
    Term* createTerm(Application*, bool);
    Term* createTerm(Abstraction*, bool);
    Atom* createAtom(Name*);
    Atom* createAtom(Variable*);
    Variable* createVariable(std::string);
    Name* createName(std::string, Expression*);
    Name* createName(std::string);


    private:
    //dont directly use this, use the method
    int id = 0;
    int ID() { return id++; }

    std::vector<Expression*> expressions;
    std::vector<Application*> applications;
    //TODO: abstratcuons will identify binding when built
    std::vector<Abstraction*> abstractions;
    std::vector<Term*> terms;
    std::vector<Atom*> atoms;
    std::vector<Variable*> variables;
    std::vector<Name*> names;
};


#endif
