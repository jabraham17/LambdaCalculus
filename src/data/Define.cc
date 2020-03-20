
#include "Define.h"

std::string Define::toJSON() {
    std::stringstream s;
    s << "\"define\":{";
    s << name->toJSON();
    s << ",";
    s << expr->toJSON();
    s << "}";
    return s.str();
}

std::ostream& operator<<(std::ostream& out, const Define& d) {

    out << *(d.name) << " = ";
    out << *(d.expr);

    return out;
}