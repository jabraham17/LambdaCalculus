#ifndef __ABSTRACTION_H__
#define __ABSTRACTION_H__

#include <string>
#include <ostream>
#include <sstream>

#include "Variable.h"
#include "Term.h"


class Term;
std::ostream& operator<<(std::ostream&, const Term&);
class Abstraction {
    private:
    int id;
    public: //TODO: make getters and setters
    Variable* variable;
    Term* term;

    public:
    Abstraction(int, Variable*, Term*);
    ~Abstraction();

    //define a copy constructor
    Abstraction(const Abstraction&);


    int ID() {
        return id;
    }

    std::string toJSON();

    //determine the binding for this abstraction
    void determineBinding();

    //rename all parameter and all bound variables
    void alphaRename(std::string);

    friend std::ostream& operator<<(std::ostream& out, const Abstraction& a) {

        out << "$" << *(a.variable) << ". " << *(a.term);
        return out;
    }

    friend bool operator==(const Abstraction& lhs, const Abstraction& rhs);
    friend bool operator!=(const Abstraction& lhs, const Abstraction& rhs);
};


#endif
