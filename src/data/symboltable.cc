#include "symboltable.h"


int ParameterID::param_id = 1;
int ParameterID::ID() {
    return param_id++;
}

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

Define* SymbolTable::createDefine(Name* name, Term* term) {
    Define* d = new Define(name, term);
    defines.push_back(d);
    return d;
}
Term* SymbolTable::createTerm(Variable* var) {
    Term* t = new Term(var);
    terms.push_back(t);
    return t;
}
Term* SymbolTable::createTerm(Name* name) {
    Term* t = new Term(name);
    terms.push_back(t);
    return t;
}
Term* SymbolTable::createTerm(Variable* var, Term* body) {
    Term* t = new Term(var, body);
    terms.push_back(t);
    return t;
}
Term* SymbolTable::createTerm(Term* a, Term* b) {
    Term* t = new Term(a, b);
    terms.push_back(t);
    return t;
}
Variable* SymbolTable::createVariable(std::string s) {
    Variable* var = new Variable(s);
    variables.push_back(var);
    return var;
}
Variable* SymbolTable::createParameter(std::string s) {
    Variable* var = new Variable(s, ParameterID::ID());
    variables.push_back(var);
    return var;
}
//only create a new one if it doesnt already exist
Name* SymbolTable::createName(std::string s) {
    Name* name = exists(s);
    //doesnt exist
    if(name == NULL) {
        name = new Name(s);
        names.push_back(name);
    }
    return name;
}
