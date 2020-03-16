
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <vector>
#include <iostream>
#include <ostream>

#include "Name.h"
#include "Term.h"
#include "Expression.h"

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
