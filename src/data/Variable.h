#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>
#include <ostream>

class Variable {
    private:
    int id;
    std::string name;

    public:
    Variable(int id, std::string name): id(id), name(name) {}
    ~Variable(){}

    friend std::ostream& operator<<(std::ostream& out, const Variable& v) {

        out << v.name;
        return out;
    }
};

#endif