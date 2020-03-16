#ifndef __ATOM_H__
#define __ATOM_H__

#include "Variable.h"
#include "Name.h"
#include <string>
#include <ostream>
#include <sstream>

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

    int ID() {
        return id;
    }

    std::string toJSON() {
        std::stringstream s;
        s << "\"atom\":{";
        if(type == NAME) s << name->toJSON();
        else if(type == VAR) s << variable->toJSON();
        s << "}";
        return s.str();
    }

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