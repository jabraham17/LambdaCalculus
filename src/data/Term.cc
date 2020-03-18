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

//if this term is an application and the application's a term is an abstraction
bool Term::isBetaRedex() {

    return type == APP && this->application->a->getType() == Term::ABS;
}

//if this is a valid redex, we can reduce it
//this is a valid redex so self is an application with 'a' as an abstraction
//therefore we can get the variable and the term from it
//the replacement term, tprime, is the 'b' of the abstraction
void applyBetaRedex(Term*& self) {

    //the abstraction we are replacing
    Abstraction* abs = self->application->a->abstraction;

    //what variable we are replacing
    Variable* replace = abs->variable;
    //the t term, must have paren
    Term* t = abs->term;
    t->addParen();

    //the t' term
    Term* tprime = self->application->b;
    //if this isnt a term add paren
    if(tprime->getType() != Term::ATOM) tprime->addParen();

    //replace all terms
    replaceVariable(t, replace, tprime);

    self = t;
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