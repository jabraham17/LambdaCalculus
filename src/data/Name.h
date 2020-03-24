#ifndef __NAME_H__
#define __NAME_H__

#include <string>
#include <ostream>
#include <sstream>

class Name {
private:
    //the name of the named expression
    std::string name;

public:
    Name(std::string);
    ~Name();

    //define a copy constructor
    Name(const Name&);

    std::string getName();


    friend std::ostream& operator<<(std::ostream& out, const Name& n);
    std::string toJSON();


    friend bool operator==(const Name&, const std::string&);
    friend bool operator!=(const Name&, const std::string&);
    friend bool operator==(const Name&, const Name&);
    friend bool operator!=(const Name&, const Name&);
};

#endif