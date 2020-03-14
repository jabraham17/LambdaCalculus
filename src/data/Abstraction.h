#ifndef __ABSTRACTION_H__
#define __ABSTRACTION_H__

#include <string>
#include <ostream>

#include "Variable.h"
#include "Term.h"


class Term;
std::ostream& operator<<(std::ostream&, const Term&);
class Abstraction {
        public:
        Variable* variable;
        Term* term;
        Abstraction(Variable* var, Term* term): variable(var), term(term) {}
        ~Abstraction(){}
        friend std::ostream& operator<<(std::ostream& out, const Abstraction& a) {

            out << "{ $" << *(a.variable) << ". " << *(a.term) << " }";
            return out;
        }
};


#endif
