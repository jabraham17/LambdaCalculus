
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
    public: //TODO: make getters
    Term* term;


    public:
    Expression(int id, Term* term): id(id), term(term) {}
    ~Expression(){}

    std::string toJSON() {
        std::stringstream s;
        s << "\"expression\":{";
        s << term->toJSON();
        s << "}";
        return s.str();
    }


    int ID() {return id;}

    void betaReduceNormalOrder();
    void betaReduceCallByValue();

    friend std::ostream& operator<<(std::ostream& out, const Expression& e) {

        out << *(e.term);

        return out;
    }
};

#endif

