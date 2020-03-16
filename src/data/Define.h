
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "Name.h"
#include "Term.h"
#include "Expression.h"

class Define {
private:
    int id;
    Name* name;
    Expression* expr;


public:
    Define(int id, Name* name, Expression* expr): id(id), name(name), expr(expr) {}
    ~Define(){}

    std::string toJSON() {
        std::stringstream s;
        s << "\"define\":{";
        s << name->toJSON();
        s << ",";
        s << expr->toJSON();
        s << "}";
        return s.str();
    }


    int ID() {
        return id;
    }

    friend std::ostream& operator<<(std::ostream& out, const Define& d) {

        out << *(d.name) << " = ";
        out << *(d.expr);

        return out;
    }
};

#endif