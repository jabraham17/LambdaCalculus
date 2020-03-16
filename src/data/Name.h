#ifndef __NAME_H__
#define __NAME_H__

#include <string>
#include <ostream>
#include <sstream>

class Name {
    private:
    int id;
    //the name of the named expression
    std::string name;

    public:
    Name(int id, std::string name): id(id), name(name) {}
    ~Name(){}

    int ID() {
        return id;
    }

    std::string toJSON() {
        std::stringstream s;
        s << "\"name\":\"" << name << "\"";
        return s.str();
    }


    friend bool operator==(const Name& lhs, const std::string& rhs) { return lhs.name == rhs; }
    friend bool operator!=(const Name& lhs, const std::string& rhs) { return !(lhs.name == rhs); }

    friend std::ostream& operator<<(std::ostream& out, const Name& n) {

        out << "@" << n.name;
        return out;
    }
};

#endif