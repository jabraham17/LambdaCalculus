
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "Name.h"
#include "Term.h"
#include "Expression.h"

class Expression;

class Define {
private:
    int id;
    Name* name;
    Expression* expr;


public:
    Define(int id, Name* name, Expression* expr): id(id), name(name), expr(expr) {}
    ~Define(){}

    Name* getName() {return name;}
    Expression* getExpression() {return expr;}

    std::string toJSON();


    int ID() {
        return id;
    }

    friend std::ostream& operator<<(std::ostream&, const Define&);
};

#endif