
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "Name.h"
#include "Term.h"

class Expression;

class Define {
private:
    Name* name;
    Term* term;

public:
    Define(Name*, Term*);
    ~Define();

    Name* getName();
    Term* getTerm();

    friend std::ostream& operator<<(std::ostream&, const Define&);
    std::string toJSON();
};

#endif