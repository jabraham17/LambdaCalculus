#include "Term.h"

Term::Term(int id, Atom* atom, bool paren): id(id), type(ATOM), paren(paren), atom(atom), abstraction(NULL), application(NULL) {}
Term::Term(int id, Abstraction* abs, bool paren): id(id), type(ABS), paren(paren), atom(NULL), abstraction(abs), application(NULL) {}
Term::Term(int id, Application* app, bool paren): id(id), type(APP), paren(paren), atom(NULL), abstraction(NULL), application(app) {}
Term::~Term(){}
Term::Term(const Term& old) {
    id = -1;
    type = old.type;
    paren = old.paren;
    if(old.atom == NULL) atom = NULL;
    else atom = new Atom(*(old.atom));

    if(old.abstraction == NULL) abstraction = NULL;
    else abstraction = new Abstraction(*(old.abstraction));

    if(old.application == NULL) application = NULL;
    else application = new Application(*(old.application));
}

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

//its a defition if its an atom and a name
bool Term::isDefinition() {
    return this->getType() == ATOM && this->atom->getType() == Atom::NAME;
}

//if this term is an application and (the application's a term is an abstraction or a definition
bool Term::isBetaRedex() {

    return type == APP && (this->application->a->getType() == Term::ABS || this->application->a->isDefinition());
}



//get a list of parameters in function (t) that contain variables bound to parameter
//parameters are the variable to a abstraction
std::vector<Term**> listOfBoundParameter(Variable** param, Term** term) {
    std::vector<Term**> res;
    //if this is an abstraction, check the param and call on the body
    if((*term)->getType() == Term::ABS) {
        //if the param is the same name as the passed in param, make a new list with one element and search the body
        if(*((*term)->abstraction->variable) == **param) {
            res.push_back(term);
        }

        std::vector<Term**> fromBody = listOfBoundParameter(param, &((*term)->abstraction->term));
        for(auto t: fromBody) {
            res.push_back(t);
        }
    }
    //if this is an application, apply to left and right
    else if((*term)->getType() == Term::APP) {
        std::vector<Term**> a = listOfBoundParameter(param, &((*term)->application->a));
        std::vector<Term**> b = listOfBoundParameter(param, &((*term)->application->b));
        for(auto t: a) {
            res.push_back(t);
        }
        for(auto t: b) {
            res.push_back(t);
        }
    }
    return res;
}


//if this is a valid redex, we can reduce it
//this is a valid redex so self is an application with 'a' as an abstraction
//therefore we can get the variable and the term from it
//the replacement term, tprime, is the 'b' of the abstraction
#include <iostream>
void applyBetaRedex(Term*& self) {

    //the function we are replacing
    Abstraction* abs = self->application->a->abstraction;

    //for all the parameters that could cause potential renaming issues, rename by appending an '_'
    for(auto t: listOfBoundParameter(&(abs->variable), &(abs->term))) {
        std::string newName = (*t)->abstraction->variable->getName() + "_";
        (*t)->abstraction->alphaRename(newName);
    }

    //what variable we are replacing, the parameter
    Variable** param = &(abs->variable);
    //the t term, must have paren
    Term* t = abs->term;
    t->addParen();

    //the t' term
    Term* tprime = self->application->b;
    //if this isnt an atom add paren
    if(tprime->getType() != Term::ATOM) tprime->addParen();

    //replace all terms
    replaceVariable(t, param, tprime);

    self = t;
}

//for all the varaiables in t, replace them with tprime(tp)
void replaceVariable(Term*& t, Variable** v, Term* tp) {
    //if its an atom and the var is bound, replace with tp
    if(t->type == Term::Type::ATOM && t->atom->getType() == Atom::VAR && t->atom->variable->isBoundTo(*v)) {
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

bool operator==(const Term& lhs, const Term& rhs) {
    if(lhs.type == rhs.type) {
        if(lhs.type == Term::ATOM) return *(lhs.atom) == *(rhs.atom);
        if(lhs.type == Term::ABS) return *(lhs.abstraction) == *(rhs.abstraction);
        if(lhs.type == Term::APP) return *(lhs.application) == *(rhs.application);
    }
    return false;
}
bool operator!=(const Term& lhs, const Term& rhs) {
    return !(lhs == rhs);
}