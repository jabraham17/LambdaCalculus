
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

#endif



//list of expressions
//expressions either named or unnamed
