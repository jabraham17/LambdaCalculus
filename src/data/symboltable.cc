#include "symboltable.h"


Name* SymbolTable::exists(std::string s) {
    //check for name
    Name* found = NULL;
    for(auto name: names) {
        //we found it, break
        if(*name == s) {
            found = name;
            break;
        }
    }
    return found;
}


Expression* SymbolTable::createExpression(Name* n, Term* t) {
    Expression* expr = new Expression(ID(), n, t);
    expressions.push_back(expr);
    return expr;
}
Expression* SymbolTable::createExpression(Term* t) {
    Expression* expr = new Expression(ID(), t);
    expressions.push_back(expr);
    return expr;
}
Application* SymbolTable::createApplication(Term* a, Term* b) {
    Application* app = new Application(ID(), a, b);
    applications.push_back(app);
    return app;
}
Abstraction* SymbolTable::createAbstraction(Variable* v, Term* t) {
    Abstraction* abs = new Abstraction(ID(), v, t);
    abstractions.push_back(abs);
    return abs;
}
Term* SymbolTable::createTerm(Atom* a, bool hasParen) {
    Term* term = new Term(ID(), a, hasParen);
    terms.push_back(term);
    return term;
}
Term* SymbolTable::createTerm(Application* a, bool hasParen) {
    Term* term = new Term(ID(), a, hasParen);
    terms.push_back(term);
    return term;
}
Term* SymbolTable::createTerm(Abstraction*a , bool hasParen) {
    Term* term = new Term(ID(), a, hasParen);
    terms.push_back(term);
    return term;
}
Atom* SymbolTable::createAtom(Name* n) {
    Atom* at = new Atom(ID(), n);
    atoms.push_back(at);
    return at;
}
Atom* SymbolTable::createAtom(Variable* v) {
    Atom* at = new Atom(ID(), v);
    atoms.push_back(at);
    return at;
}
Variable* SymbolTable::createVariable(std::string s) {
    Variable* var = new Variable(ID(), s);
    variables.push_back(var);
    return var;
}
//only create a new one if it doesnt already exist
Name* SymbolTable::createName(std::string s, Expression* e) {
    Name* name = exists(s);

    //doesnt exist
    if(name == NULL) {
        name = new Name(ID(), s, e);
        names.push_back(name);
    }

    return name;
}
Name* SymbolTable::createName(std::string s) {
    return createName(s, NULL);
}

