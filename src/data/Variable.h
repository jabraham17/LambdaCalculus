#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>
#include <ostream>
#include <sstream>

class Variable {
private:
    std::string name;
    //the parameter this variable is bound to
    Variable* boundTo;
    //if its a parameter, it has an id
    bool isParam;
    int id;

public:
    //all variables are assumed free at initialization
    Variable(std::string);
    Variable(std::string, int);
    ~Variable();
    //define a copy constructor
    Variable(const Variable&);

    void setName(std::string s);
    std::string getName();

    void setBoundTo(Variable* v);
    Variable* getBoundTo();
    bool isBound();
    bool isFree();
    void makeParameter(int);
    bool isParameter();
    int getID();

    //determine if this is bound to var v
    //v is the parameter of the abstraction
    bool isBoundTo(Variable* v);


    friend std::ostream& operator<<(std::ostream&, const Variable&);
    std::string toJSON();

    friend bool operator==(const Variable&, const Variable&);
    friend bool operator!=(const Variable&, const Variable&);
};

#endif