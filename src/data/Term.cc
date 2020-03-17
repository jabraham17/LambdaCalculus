#include "Term.h"

std::string Term::toJSON() {
    std::stringstream s;
    s << "\"term\":{";
    if(hasParen()) s << "\"parentheses\":true,";
    if(isBetaRedex()) s << "\"beta-redex\":true,";

    if(type == ATOM) s << atom->toJSON();
    else if(type == ABS) s << abstraction->toJSON();
    else if(type == APP) s << application->toJSON();
    s << "}";
    return s.str();
}

//if this term is an application and the application is a redex, then this term is a redex
bool Term::isBetaRedex() {
    return type == APP && application->isBetaRedex();
}

//if this is a valid redex, we can reduce it
//the body of the applications 'a' term will be the new term
//the replacement is the applications 'b' term
//TODO: does the right thing, just leaves behind the application var it replaced
//TODO: fix, does it wrong
void applyBetaRedex(Term*& self) {

    //the abstraction we are replacing
    Abstraction* abs = findAbstraction(self->application->a);

    //what variable we are replacing
    Variable* replace = abs->variable;
    //the t term, must have paren
    Term* t = self->application->a;
    t->hasParen();

    //the t' term, also must have paren
    Term* tprime = self->application->b;
    tprime->hasParen();

    //replace all terms
    replaceVariable(t, replace, tprime);

    self = t;
}

//find the lowest leftmost abstraction in a series of applications
Abstraction* findAbstraction(Term* self) {
    if(self->getType() == Term::Type::ABS) {
        return self->abstraction;
    }
    //find the buried type in an appplciation
    else if(self->getType() == Term::Type::APP) {
        return findAbstraction(self->application->a);
    }
    //not an app or an abs
    else {
        return NULL;
    }
}

//for all the varaiables in t, replace them with tprime(tp)
void replaceVariable(Term*& t, Variable* v, Term* tp) {
    //if its an atom and the vars match, replace with tp
    if(t->type == Term::Type::ATOM && t->atom->type == Atom::Type::VAR && *(t->atom->variable) == *v) {
        t = tp;
    }
    //if an abstraction, call recursivly on abstraction body
    else if(t->type == Term::Type::ABS) {
        replaceVariable(t->abstraction->term, v, tp);
    }
    //if an application, call recursivly on both terms
    else if(t->type == Term::Type::APP) {
        replaceVariable(t->application->a, v, tp);
        replaceVariable(t->application->b, v, tp);
    }
}