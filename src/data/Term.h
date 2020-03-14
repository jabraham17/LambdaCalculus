#ifndef __TERM_H__
#define __TERM_H__

#include <string>
#include <ostream>

#include "Application.h"
#include "Abstraction.h"
#include "Atom.h"


std::ostream& operator<<(std::ostream&, const Abstraction&);
std::ostream& operator<<(std::ostream&, const Application&);
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

#endif