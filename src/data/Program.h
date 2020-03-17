#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <string>
#include <ostream>
#include <sstream>

#include "Expression.h"
#include "symboltable.h"
#include "Define.h"

class Program {
    private:
    //TODO: make these private and add accesors
    public:
    std::vector<Expression*> statements;
    SymbolTable* table;
    std::vector<Define*> library;

    public:
    Program(): statements(), table(new SymbolTable()), library() {}
    ~Program() {
        delete(table);
    }
    friend std::ostream& operator<<(std::ostream& out, const Program& p) {
        std::string sep;
        for(size_t i = 0; i < p.statements.size(); i++) {
            out << sep << (i+1) << ": " << *(p.statements[i]);
            sep = '\n';
        }
        return out;
    }

    std::string toJSON();

    void readLibrary(std::string);

};

#endif