#ifndef __TERM_H__
#define __TERM_H__

#include <string>
#include <ostream>
#include <sstream>

#include "Application.h"
#include "Abstraction.h"
#include "Atom.h"


class Abstraction;
class Application;
std::ostream& operator<<(std::ostream&, const Abstraction&);
std::ostream& operator<<(std::ostream&, const Application&);
class Term {
    private:
    int id;
    enum Type { ATOM=0, ABS, APP};
    Type type;
    //whether the use decided to put parentheses around this
    bool paren;
    Atom* atom;
    Abstraction* abstraction;
    Application* application;

    public:
    Term(int id, Atom* atom, bool paren): id(id), type(ATOM), paren(paren), atom(atom), abstraction(NULL), application(NULL) {}
    Term(int id, Abstraction* abs, bool paren): id(id), type(ABS), paren(paren), atom(NULL), abstraction(abs), application(NULL) {}
    Term(int id, Application* app, bool paren): id(id), type(APP), paren(paren), atom(NULL), abstraction(NULL), application(app) {}
    ~Term(){}


    std::string toJSON();


    int ID() {
        return id;
    }

    void addParen() { paren = true; }
    bool hasParen() {return paren; }


    friend std::ostream& operator<<(std::ostream& out, const Term& t) {

        if(t.paren) out << "(";

        if(t.type == ATOM) {
            out << "<" << *(t.atom) << ">";
        }
        else if(t.type == ABS) {
            out << *(t.abstraction);
        }
        else if(t.type == APP) {
            out << *(t.application);
        }

        if(t.paren) out << ")";

        return out;
    }
};

#endif