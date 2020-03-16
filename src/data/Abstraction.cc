# include "Abstraction.h"

std::string Abstraction::toJSON() {
    std::stringstream s;
    s << "\"abstraction\":{";
    s << variable->toJSON();
    s << ",";
    s << term->toJSON();
    s << "}";
    return s.str();
}