
#ifndef __EXPRESSION_H_
#define __EXPRESSION_H_

#include <string>
#include <vector>
#include <iostream>
#include <ostream>

class Expression;
class Name {
    public:
    //the name of the named expression
    std::string name;
    //the expression the name refers to
    Expression* expression;

    Name(std::string name, Expression* expr): name(name), expression(expr) {}
    ~Name(){}

    friend std::ostream& operator<<(std::ostream& out, const Name& n) {

        out << "@" << n.name;
        return out;
    }
};

class Variable {
    public:
    std::string name;
    Variable(std::string name): name(name) {}
    ~Variable(){}

    friend std::ostream& operator<<(std::ostream& out, const Variable& v) {

        out << v.name;
        return out;
    }
};

class Atom {
    public:
    enum Type { NAME=0, VAR };
    Type type;
    Name* name;
    Variable* variable;
    Atom(Name* name): type(NAME), name(name), variable(NULL) {}
    Atom(Variable* var): type(VAR), name(NULL), variable(var) {}
    ~Atom(){}

    friend std::ostream& operator<<(std::ostream& out, const Atom& a) {

        if(a.type == NAME) {
            out << *(a.name);
        }
        else if(a.type == VAR) {
            out << *(a.variable);
        }
        return out;
    }
};

class Application;
class Abstraction;

std::ostream& operator<<(std::ostream& out, const Application& t);
std::ostream& operator<<(std::ostream& out, const Abstraction& t);

class Term {
    public:
    enum Type { ATOM=0, ABS, APP};
    Type type;
    Atom* atom;
    Abstraction* abstraction;
    Application* application;
    Term(Atom* atom): type(ATOM), atom(atom), abstraction(NULL), application(NULL) {}
    Term(Abstraction* abs): type(ABS), atom(NULL), abstraction(abs), application(NULL) {}
    Term(Application* app): type(APP), atom(NULL), abstraction(NULL), application(app) {}
    ~Term(){}

    friend std::ostream& operator<<(std::ostream& out, const Term& t) {

        if(t.type == ATOM) {
            out << "<" << *(t.atom) << ">";
        }
        else if(t.type == ABS) {
            out << *(t.abstraction);
        }
        else if(t.type == APP) {
            out << *(t.application);
        }
        return out;
    }
};

class Abstraction {
    public:
    Variable* variable;
    Term* term;
    Abstraction(Variable* var, Term* term): variable(var), term(term) {}
    ~Abstraction(){}
    friend std::ostream& operator<<(std::ostream& out, const Abstraction& a) {

        out << "{ $" << *(a.variable) << ". " << *(a.term) << " }";
        return out;
    }
};

class Application {
    public:
    Term* a;
    Term* b;
    Application(Term* a, Term* b): a(a), b(b) {}
    ~Application(){}
    friend std::ostream& operator<<(std::ostream& out, const Application& a) {
        out << "[ " << *(a.a) << " " << *(a.b) << " ]";
        return out;
    }
};

class Expression {
    public:
    bool hasName;
    Name* name;
    Term* term;
    Expression(Name* name, Term* term): hasName(true), name(name), term(term) {}
    Expression(Term* term): hasName(false), name(NULL), term(term) {}
    ~Expression(){}

    friend std::ostream& operator<<(std::ostream& out, const Expression& e) {

        if(e.hasName) {
            out << *(e.name) << " = ";
        }
        out << *(e.term);

        return out;
    }
};

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

class Program {
    public:
    std::vector<Expression*> statements;
    SymbolTable* table;
    ~Program() {
        delete(table);
    }
    friend std::ostream& operator<<(std::ostream& out, const Program& p) {
        std::string sep;
        for(int i = 0; i < p.statements.size(); i++) {
            out << sep << (i+1) << ": " << *(p.statements[i]);
            sep = '\n';
        }
        return out;
    }
};

#endif



//list of expressions
//expressions either named or unnamed
