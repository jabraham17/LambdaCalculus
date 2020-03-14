#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <string>
#include <ostream>

#include "Term.h"

class Term;
std::ostream& operator<<(std::ostream&, const Term&);
class Application {
public:
    Term* a;
    Term* b;
    Application(Term* a, Term* b): a(a), b(b) {}
    ~Application(){}
    friend std::ostream& operator<<(std::ostream& out, const Application& a) {
        out << "[ " << *(a.a) << " " << *(a.b) << " ]";
        return out;
    }
};

#endif