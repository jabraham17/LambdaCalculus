#ifndef __NAME_H__
#define __NAME_H__

#include "Expression.h"
#include <string>
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

#endif