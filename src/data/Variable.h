#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>
#include <ostream>
#include <sstream>

class Variable {
    private:
    int id;
    std::string name;
    //the parameter this variable is bound to
    Variable* boundTo;

    public:
    //all variables are assumed free at initialization
    Variable(int id, std::string name): id(id), name(name), boundTo(NULL) {}
    ~Variable(){}

    int ID() {
        return id;
    }

    void setName(std::string s) {name = s;}
    std::string getName() {return name;}

    void setBoundTo(Variable* v) {boundTo = v;}
    Variable* getBoundTo() {return boundTo;}
    bool isBound() {return boundTo != NULL;}
    bool isFree() {return !isBound();}

    //determine if this is bound to var v
    //v is the parameter of the abstraction
    bool isBoundTo(Variable* v) {
        return boundTo != NULL && (*v == *boundTo) && v == boundTo;
    }

    std::string toJSON() {
        std::stringstream s;
        s << "\"variable\":\"" << name <<  "\"";
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