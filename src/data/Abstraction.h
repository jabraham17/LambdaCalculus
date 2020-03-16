#ifndef __ABSTRACTION_H__
#define __ABSTRACTION_H__

#include <string>
#include <ostream>

#include "Variable.h"
#include "Term.h"


class Term;
std::ostream& operator<<(std::ostream&, const Term&);
class Abstraction {
    private:
    int id;
    Variable* variable;
    Term* term;

    public:
    Abstraction(int id, Variable* var, Term* term): id(id), variable(var), term(term) {}
    ~Abstraction(){}
    friend std::ostream& operator<<(std::ostream& out, const Abstraction& a) {

        out << "{ $" << *(a.variable) << ". " << *(a.term) << " }";
        return out;
    }
};


#endif