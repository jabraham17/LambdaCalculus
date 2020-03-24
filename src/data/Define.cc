
#include "Define.h"

Define::Define(Name* name, Term* term): name(name), term(term) {}
Define::~Define(){}

Name* Define::getName() {return name;}
Term* Define::getTerm() {return term;}

std::ostream& operator<<(std::ostream& out, const Define& d) {

    out << *(d.name) << " = ";
    out << *(d.term);

    return out;
}
std::string Define::toJSON() {
    std::stringstream s;
    s << "\"define\":{";
    s << name->toJSON();
    s << ",";
    s << term->toJSON();
    s << "}";
    return s.str();
}