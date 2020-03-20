#include "Abstraction.h"

Abstraction::Abstraction(int id, Variable* var, Term* term): id(id), variable(var), term(term) {}
Abstraction::~Abstraction(){}

//define a copy constructor
Abstraction::Abstraction(const Abstraction& old) {
    id = -1;
    variable = new Variable(*(old.variable));
    term = new Term(*(old.term));
}


std::string Abstraction::toJSON() {
    std::stringstream s;
    s << "\"abstraction\":{";
    s << variable->toJSON();
    s << ",";
    s << term->toJSON();
    s << "}";
    return s.str();
}

//determine the binding for this abstraction
//go through all the terms in an abstraction
//we call this terms (abstarction) variable the parameter
//if term is a variable that matches the parameter, it is bound
//if the term is a abstarction with a variable that matches the parameter, do not bind anything inside of it
void determineBinding(Variable** param, Term** term) {

    //if this is a variable that is the same as the parameter
    //this is a base case
    if((*term)->getType() == Term::ATOM && (*term)->atom->getType() == Atom::VAR && *((*term)->atom->variable) == **param) {
        (*term)->atom->variable->setBoundTo(*param);
        return;
    }
    //if this is an abstraction and the abstractions variable is the same as the parameter, skip
    //otherwise process the abstraction body
    if((*term)->getType() == Term::ABS) {
        if(*((*term)->abstraction->variable) == **param) return;
        else {
            determineBinding(param, &((*term)->abstraction->term));
            return;
        }
    }
    //if this is an application, apply to left and right
    if((*term)->getType() == Term::APP) {
        determineBinding(param, &((*term)->application->a));
        determineBinding(param, &((*term)->application->b));
        return;
    }
}



void Abstraction::determineBinding() {
    Variable** param = &(this->variable);
    Term** term = &(this->term);
    ::determineBinding(param, term);
}


void alphaRename(std::string newName, Variable** param, Term** term) {
    //if this is a variable that is bound to the param, rename it
    if((*term)->getType() == Term::ATOM && (*term)->atom->VAR == Atom::VAR && (*term)->atom->variable->isBoundTo(*param)) {

        (*term)->atom->variable->setName(newName);
        return;
    }
    //if this is an abstraction, call on the body
    if((*term)->getType() == Term::ABS) {
        alphaRename(newName, param, &((*term)->abstraction->term));
    }
    //if this is an application, apply to left and right
    if((*term)->getType() == Term::APP) {
        alphaRename(newName, param, &((*term)->application->a));
        alphaRename(newName, param, &((*term)->application->b));
        return;
    }
}

//rename all parameter and all bound variables
void Abstraction::alphaRename(std::string newName) {
    Variable** param = &(this->variable);
    Term** term = &(this->term);
    ::alphaRename(newName, param, term);
    //now set the param to the newName
    (*param)->setName(newName);

}

bool operator==(const Abstraction& lhs, const Abstraction& rhs) {
    return *(lhs.variable) == *(rhs.variable) && *(lhs.term) == *(rhs.term);
}
bool operator!=(const Abstraction& lhs, const Abstraction& rhs) {
    return !(lhs == rhs);
}