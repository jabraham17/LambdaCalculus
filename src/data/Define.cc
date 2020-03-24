
#include "Define.h"

Define(Name* name, Term* term): name(name), term(term) {}
~Define(){}

Name* getName() {return name;}
Term* getTerm() {return term;}

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