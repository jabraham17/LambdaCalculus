#include "Application.h"

std::string Application::toJSON() {
    std::stringstream s;
    s << "\"application\":[";

    s << "{" << a->toJSON() << "},";
    s << "{" << b->toJSON() << "}";

    s << "]";
    return s.str();
}

#include <iostream>

//its a beta redex if term a is an abstraction
//however, this abstraction could be buried in an application
bool Application::isBetaRedex() {

    if(this->a->getType() == Term::Type::ABS) {
        return true;
    }
    //find the buried type in an appplciation
    else if(this->a->getType() == Term::Type::APP) {
        return this->a->isBetaRedex();
    }
    //not an app or an abs
    else {
        return false;
    }
}