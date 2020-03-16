#ifndef __ATOM_H__
#define __ATOM_H__

#include "Variable.h"
#include "Name.h"
#include <string>
#include <ostream>

class Atom {
    private:
    int id;
    enum Type { NAME=0, VAR };
    Type type;
    Name* name;
    Variable* variable;

    public:
    Atom(int id, Name* name): id(id), type(NAME), name(name), variable(NULL) {}
    Atom(int id, Variable* var): id(id), type(VAR), name(NULL), variable(var) {}
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

#endif