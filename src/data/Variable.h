#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>
#include <ostream>
#include <sstream>

class Variable {
    private:
    int id;
    std::string name;

    public:
    Variable(int id, std::string name): id(id), name(name) {}
    ~Variable(){}

    int ID() {
        return id;
    }

    std::string toJSON() {
        std::stringstream s;
        s << "\"variable\":\"" << name << "\"";
        return s.str();
    }

    friend std::ostream& operator<<(std::ostream& out, const Variable& v) {
        out << v.name;
        return out;
    }

    friend bool operator==(const Variable& lhs, const Variable& rhs) { return lhs.name == rhs.name; }
    friend bool operator!=(const Variable& lhs, const Variable& rhs) { return !(lhs == rhs); }
};

#endif