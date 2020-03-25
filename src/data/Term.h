#ifndef __TERM_H__
#define __TERM_H__

#include <string>
#include <ostream>
#include <sstream>

#include <vector>
#include "Variable.h"
#include "Name.h"


enum TermType { VARIABLE=0, NAME, ABSTRACTION, APPLICATION};
std::string getTermTypeName(TermType t);

class Term {
    private:
    TermType type;
    //whether the use decided to put parentheses around this
    bool paren;
    Variable* variable;
    Name* name;
    Term* t1;
    Term* t2;

    public:
    Term(Variable*);
    Term(Name*);
    Term(Term*, Term*);
    Term(Variable*, Term*);
    ~Term();
    //define a copy constructor
    Term(const Term&);

    TermType getType() const;
    bool isType(TermType) const;
    void addParen();
    bool hasParen() const;
    Variable* getVariable() const;
    Name* getName() const;
    Term* getTermA() const;
    Term* getTermB() const;
    Term* getBody() const;
    Variable*& getVariable();
    Name*& getName();
    Term*& getTermA();
    Term*& getTermB();
    Term*& getBody();

    bool isDefinition();
    bool isBetaRedex();
    //its a value if not reducible
    bool isValue();
    friend void applyBetaRedex(Term*&);
    friend void replaceVariable(Term*&, Variable**, Term*);

    //determine the binding for the vars in this
    void determineBinding();
    friend void determineBinding(Variable*&, Term*&);


    friend std::ostream& operator<<(std::ostream& out, const Term& t);
    std::string toJSON();

    friend bool operator==(const Term& lhs, const Term& rhs);
    friend bool operator!=(const Term& lhs, const Term& rhs);
};

#endif