#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <string>
#include <ostream>

#include "Expression.h"
#include "symboltable.h"

class Program {
    private:
    //TODO: make these private and add accesors
    public:
    std::vector<Expression*> statements;
    SymbolTable* table;

    public:
    Program(): statements(), table(new SymbolTable()) {}
    ~Program() {
        delete(table);
    }
    friend std::ostream& operator<<(std::ostream& out, const Program& p) {
        std::string sep;
        for(int i = 0; i < p.statements.size(); i++) {
            out << sep << (i+1) << ": " << *(p.statements[i]);
            sep = '\n';
        }
        return out;
    }

    //TODO
    std::string createASTNode();
};

#endif