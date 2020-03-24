
#include "Name.h"

Name::Name(std::string name): name(name) {}
Name::~Name(){}
Name::Name(const Name& old) {
    name = old.name;
}

std::string Name::getName() {return name;}


std::ostream& operator<<(std::ostream& out, const Name& n) {
    out << "@" << n.name;
    return out;
}
std::string Name::toJSON() {
    std::stringstream s;
    s << "\"name\":\"" << name << "\"";
    return s.str();
}


bool operator==(const Name& lhs, const std::string& rhs) { return lhs.name == rhs; }
bool operator!=(const Name& lhs, const std::string& rhs) { return !(lhs.name == rhs); }
bool operator==(const Name& lhs, const Name& rhs) { return lhs.name == rhs.name; }
bool operator!=(const Name& lhs, const Name& rhs) { return !(lhs == rhs); }
