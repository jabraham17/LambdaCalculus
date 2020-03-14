#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>
#include <ostream>

class Variable {
public:
    std::string name;
    Variable(std::string name): name(name) {}
    ~Variable(){}

    friend std::ostream& operator<<(std::ostream& out, const Variable& v) {

        out << v.name;
        return out;
    }
};

#endif