
#ifndef __EXPRESSION_H_
#define __EXPRESSION_H_

#include <string>
#include <vector>

class Name {
    //the name of the named expression
    std::string name;
    //the expression the name refers to
    Expression* expression;
};

class Variable {
    std::string name;
};

class Atom {
    enum Type { NAME=0, VAR };
    Type type;
    Name* name;
    Variable* variable;
};

class Term {
    enum Type { ATOM=0, ABS, APP};
    Type type;
    Atom* atom;
    Abstraction* abstraction;
    Application* application;
    bool isParenthesized;
};

class Abstraction {
    Variable* variable;
    Term* term;
};

class Application {
    Term* a;
    Term* b;
};

class Expression {
    bool hasName;
    Name* name;
    Term* term;
};

class SymbolTable {
    std::vector<Expression*> expressions;
    std::vector<Application*> applications;
    std::vector<Abstraction*> abstractions;
    std::vector<Term*> terms;
    std::vector<Atom*> atoms;
    std::vector<Variable*> variables;
    std::vector<Name*> names;
};

#endif



//list of expressions
//expressions either named or unnamed
