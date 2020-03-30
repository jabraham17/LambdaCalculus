#include "Variable.h"


//all variables are assumed free at initialization
Variable::Variable(std::string name): name(name), boundTo(NULL), isParam(false), id(-1) {}
Variable::Variable(std::string name, int id): name(name), boundTo(NULL), isParam(true), id(id) {}
Variable::~Variable(){}

//define a copy constructor
Variable::Variable(const Variable& old) {
    name = old.name;
    boundTo = old.boundTo;
    isParam = old.isParam;
    id = old.id;
}

void Variable::setName(std::string s) {name = s;}
std::string Variable::getName() {return name;}

void Variable::setBoundTo(Variable* v) {boundTo = v;}
Variable* Variable::getBoundTo() {return boundTo;}
bool Variable::isBound() {return boundTo != NULL;}
bool Variable::isFree() {return !isBound();}
void Variable::makeParameter(int id) {this->id = id; isParam = true;}
bool Variable::isParameter() {return isParam;}
int Variable::getID() {return id;};

//determine if this is bound to a param
//its bound if the param if the ids match
bool Variable::isBoundTo(Variable* param) {return boundTo != NULL && (param->id == boundTo->id);}


std::ostream& operator<<(std::ostream& out, const Variable& v) {
    out << v.name;
    return out;
}
std::string Variable::toJSON() {
    std::stringstream s;
    if(isBound())
        s << "\"variable\":{\"variable\":\"" << name << "\",\"boundTo\":\"" << this->getBoundTo()->getID() << "\"}";
    else if(isParameter())
        s << "\"variable\":{\"variable\":\"" << name << "\",\"id\":\"" << this->getID() << "\"}";
    else
        s << "\"variable\":\"" << name <<  "\"";
    return s.str();
}

bool operator==(const Variable& lhs, const Variable& rhs) {return lhs.name == rhs.name;}
bool operator!=(const Variable& lhs, const Variable& rhs) {return !(lhs == rhs);}

