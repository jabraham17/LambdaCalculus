#include "Term.h"

std::string getTermTypeName(TermType t) {
    std::string names[] = {"VARIABLE", "NAME", "ABSTRACTION", "APPLICATION"};
    return names[t];
}

Term::Term(Variable* var): type(VARIABLE), variable(var), name(NULL), t1(NULL), t2(NULL) {}
Term::Term(Name* name): type(NAME), variable(NULL), name(name), t1(NULL), t2(NULL) {}
Term::Term(Term* a, Term* b): type(APPLICATION), variable(NULL), name(NULL), t1(a), t2(b) {}
Term::Term(Variable* var, Term* body): type(ABSTRACTION), variable(var), name(NULL), t1(body), t2(NULL) {
    //determine the binding of the abstratction
    determineBinding();
}
Term::~Term(){}
Term::Term(const Term& old) {
    type = old.type;
    paren = old.paren;

    if(old.variable == NULL) variable = NULL;
    else variable = new Variable(*(old.variable));

    if(old.name == NULL) name = NULL;
    else name = new Name(*(old.name));

    if(old.t1 == NULL) t1 = NULL;
    else t1 = new Term(*(old.t1));

    if(old.t2 == NULL) t2 = NULL;
    else t2 = new Term(*(old.t2));
}

TermType Term::getType() const {return type;}
bool Term::isType(TermType t) const {return type == t;}
void Term::addParen() {paren = true;}
bool Term::hasParen() const {return paren;}
Variable* Term::getVariable() const {return variable;}
Name* Term::getName() const {return name;}
Term* Term::getTermA() const {return t1;}
Term* Term::getTermB() const {return t2;}
Term* Term::getBody() const {return t1;}
Variable*& Term::getVariable() {return variable;}
Name*& Term::getName() {return name;}
Term*& Term::getTermA() {return t1;}
Term*& Term::getTermB() {return t2;}
Term*& Term::getBody() {return t1;}

//its a definition if its a name
bool Term::isDefinition() {return isType(NAME);}
//if this term is an application and (the application's a term is an abstraction
bool Term::isBetaRedex() {return isType(APPLICATION) && getTermA()->isType(ABSTRACTION);}
//its a value if not reducible
bool Term::isValue() {return !isBetaRedex();}

//get a list of parameters in function (t) that contain variables bound to parameter
//parameters are the variable to a abstraction
//TODO
/*std::vector<Term**> listOfBoundParameter(Variable** param, Term** term) {
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
//TODO
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
//TODO
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
}*/


//determine the binding for this abstraction
//go through all the terms in an abstraction
//we call this terms (abstarction) variable the parameter
//if term is a variable that matches the parameter, it is bound
//if the term is a abstarction with a variable that matches the parameter, do not bind anything inside of it
void determineBinding(Variable*& param, Term*& term) {

    //if this is a variable that is the same as the parameter
    //this is a base case
    if(term->isType(VARIABLE) && *param == *(term->getVariable())) {
        term->getVariable()->setBoundTo(param);
    }
    //if this is an abstraction and the abstractions variable is not same as the parameter
    //process the abstraction body
    else if(term->isType(ABSTRACTION) && *(term->getVariable()) != *param) {
        determineBinding(param, term->getBody());
    }
    //if this is an application, apply to left and right
    else if(term->isType(APPLICATION)) {
        determineBinding(param, term->getTermA());
        determineBinding(param, term->getTermB());
    }
}
void Term::determineBinding() {
    ::determineBinding(this->getVariable(), this->getBody());
}


void alphaRename(std::string newName, Variable*& param, Term*& term) {
    //if this is a variable that is bound to the param, rename it
    if(term->isType(VARIABLE) && term->variable->isBoundTo(param)) {
        term->variable->setName(newName);
    }
        //if this is an abstraction, call on the body
    else if(term->isType(ABSTRACTION)) {
        alphaRename(newName, param, term->getBody());
    }
        //if this is an application, apply to left and right
    else if(term->isType(APPLICATION)) {
        alphaRename(newName, param, term->getTermA());
        alphaRename(newName, param, term->getTermA());
    }
}
//this is an abstraction, we rename parameter and all vars bound to it
void Term::alphaRename(std::string newName) {
    //rename all params in the body
    ::alphaRename(newName, this->variable, this->getBody());
    //now set the param to the newName
    this->variable->setName(newName);
}


std::ostream& operator<<(std::ostream& out, const Term& t) {

    if(t.paren) out << "(";

    if(t.isType(VARIABLE)) out << *(t.variable);
    else if(t.isType(NAME)) out << *(t.name);
    else if(t.isType(APPLICATION)) out << *(t.getTermA()) << " " << *(t.getTermB());
    else if(t.isType(ABSTRACTION)) out << "λ" << *(t.variable) << ". " << *(t.getBody());

    if(t.paren) out << ")";

    return out;
}
std::string Term::toJSON() {
    std::stringstream s;

    if(isBetaRedex()) s << "\"beta-redex\":true,";
    if(hasParen()) s << "\"parentheses\":true,";

    if(isType(VARIABLE)) s << variable->toJSON();
    else if(isType(NAME)) s << name->toJSON();
    else if(isType(ABSTRACTION)) {
        s << "\"abstraction\":{";
        s << variable->toJSON();
        s << ",\"body\":{" << getBody()->toJSON() << "}}";
    }
    else if(isType(APPLICATION)) {
        s << "\"application\":[";
        s << "{" << getTermA()->toJSON() << "},{" << getTermB()->toJSON() << "}]";
    }

    /*s << "\"term\":{";
    s << "\"type\":\"" << getTermTypeName(type) << "\",";
    if(hasParen()) s << "\"parentheses\":true,";
    if(isBetaRedex()) s << "\"beta-redex\":true,";

    if(isType(VARIABLE))
        if (variable->isBound())
            s << "\"variable\":{" << variable->toJSON() << ",\"boundTo\":\"" << variable->getBoundTo()->getName() << "\"}";
        else s << variable->toJSON();
    else if(isType(NAME)) s << name->toJSON();
    else if(isType(APPLICATION)) s << "\"A\":{" << getTermA()->toJSON() << "},\"B\":{" << getBody()->toJSON() << "}";
    else if(isType(ABSTRACTION)) s << variable->toJSON() << "," << getBody()->toJSON();

    s << "}";*/
    return s.str();
}

bool operator==(const Term& lhs, const Term& rhs) {
    if(lhs.type == rhs.type) {
        if(lhs.isType(VARIABLE)) return *(lhs.variable) == *(rhs.variable);
        if(lhs.isType(NAME)) return *(lhs.name) == *(rhs.name);
        if(lhs.isType(ABSTRACTION)) return *(lhs.variable) == *(rhs.variable) && *(lhs.getBody()) == *(rhs.getBody());
        if(lhs.isType(APPLICATION)) return *(lhs.getTermA()) == *(rhs.getTermA()) && *(lhs.getTermB()) == *(rhs.getTermB());
    }
    return false;
}

bool operator!=(const Term& lhs, const Term& rhs) {
    return !(lhs == rhs);
}