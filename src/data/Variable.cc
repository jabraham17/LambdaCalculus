#include "Variable.h"


//all variables are assumed free at initialization
Variable::Variable(std::string name): name(name), boundTo(NULL) {}
Variable::~Variable(){}

//define a copy constructor
Variable::Variable(const Variable& old) {
    name = old.name;
    boundTo = old.boundTo;
}

void Variable::setName(std::string s) {name = s;}
std::string Variable::getName() {return name;}

void Variable::setBoundTo(Variable* v) {boundTo = v;}
Variable* Variable::getBoundTo() {return boundTo;}
bool Variable::isBound() {return boundTo != NULL;}
bool Variable::isFree() {return !isBound();}

//determine if this is bound to var v
//v is the parameter of the abstraction
bool Variable::isBoundTo(Variable* v) {return boundTo != NULL && (*v == *boundTo);}


std::ostream& operator<<(std::ostream& out, const Variable& v) {
    out << v.name;
    return out;
}
std::string Variable::toJSON() {
    std::stringstream s;
    s << "\"variable\":\"" << name <<  "\"";
    return s.str();
}

bool operator==(const Variable& lhs, const Variable& rhs) {return lhs.name == rhs.name;}
bool operator!=(const Variable& lhs, const Variable& rhs) {return !(lhs == rhs);}

