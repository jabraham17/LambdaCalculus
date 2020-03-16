
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "Name.h"
#include "Term.h"

class Expression {
    private:
    int id;
    bool hasName;
    Name* name;
    Term* term;


    public:
    Expression(int id, Name* name, Term* term): id(id), hasName(true), name(name), term(term) {}
    Expression(int id, Term* term): id(id), hasName(false), name(NULL), term(term) {}
    ~Expression(){}

    std::string toJSON() {
        std::stringstream s;
        s << "\"expression\":{";
        s << name->toJSON();
        s << ",";
        s << term->toJSON();
        s << "}";
        return s.str();
    }


    int ID() {
        return id;
    }

    friend std::ostream& operator<<(std::ostream& out, const Expression& e) {

        if(e.hasName) {
            out << *(e.name) << " = ";
        }
        out << *(e.term);

        return out;
    }
};

#endif



//list of expressions
//expressions either named or unnamed
