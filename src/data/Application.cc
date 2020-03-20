#include "Application.h"


Application::Application(int id, Term* a, Term* b): id(id), a(a), b(b) {}
Application::~Application(){}

//define a copy constructor
Application::Application(const Application& old) {
    id = -1;
    a = new Term(*(old.a));
    b = new Term(*(old.b));
}

std::string Application::toJSON() {
    std::stringstream s;
    s << "\"application\":[";

    s << "{" << a->toJSON() << "},";
    s << "{" << b->toJSON() << "}";

    s << "]";
    return s.str();
}