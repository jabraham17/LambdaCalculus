#ifndef __NAME_H__
#define __NAME_H__

#include "Expression.h"
#include <string>
#include <ostream>

class Expression;
class Name {
    private:
    int id;
    //the name of the named expression
    std::string name;
    //the expression the name refers to
    Expression* expression;

    public:
    Name(int id, std::string name, Expression* expr): id(id), name(name), expression(expr) {}
    ~Name(){}

    void setExpression(Expression* expr) {expression = expr;}

    friend bool operator==(const Name& lhs, const std::string& rhs) { return lhs.name == rhs; }
    friend bool operator!=(const Name& lhs, const std::string& rhs) { return !(lhs.name == rhs); }

    friend std::ostream& operator<<(std::ostream& out, const Name& n) {

        out << "@" << n.name;
        return out;
    }
};

#endif