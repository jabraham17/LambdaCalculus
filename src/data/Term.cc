#include "Term.h"

std::string Term::toJSON() {
    std::stringstream s;
    s << "\"term\":{";
    if(hasParen()) s << "\"parentheses\":true,";

    if(type == ATOM) s << atom->toJSON();
    else if(type == ABS) s << abstraction->toJSON();
    else if(type == APP) s << application->toJSON();
    s << "}";
    return s.str();
}