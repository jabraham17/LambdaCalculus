
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "Name.h"
#include "Term.h"
#include "Define.h"

class Define;
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

    void betaReduceNormalOrder(std::vector<Define*>);
    bool betaReduceNormalOrderStep(std::vector<Define*>);
    void betaReduceCallByValue();
    bool betaReduceCallByValueStep();

    friend std::ostream& operator<<(std::ostream&, const Expression&);
};

#endif

