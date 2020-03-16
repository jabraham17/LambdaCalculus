#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <string>
#include <ostream>

#include "Term.h"
#include <string>
#include <ostream>
#include <sstream>

class Term;

std::ostream& operator<<(std::ostream&, const Term&);
class Application {
    private:
    int id;
    Term* a;
    Term* b;

    public:
    Application(int id, Term* a, Term* b): id(id), a(a), b(b) {}
    ~Application(){}

    int ID() {
        return id;
    }

    std::string toJSON();

    friend std::ostream& operator<<(std::ostream& out, const Application& a) {
        out << "[ " << *(a.a) << " " << *(a.b) << " ]";
        return out;
    }
};

#endif