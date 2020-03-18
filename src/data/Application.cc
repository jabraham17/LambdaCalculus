#include "Application.h"

std::string Application::toJSON() {
    std::stringstream s;
    s << "\"application\":[";

    s << "{" << a->toJSON() << "},";
    s << "{" << b->toJSON() << "}";

    s << "]";
    return s.str();
}