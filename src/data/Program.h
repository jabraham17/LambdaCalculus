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
        for(int i = 0; i < (int)p.statements.size(); i++) {
            out << sep << (i+1) << ": " << *(p.statements[i]);
            sep = '\n';
        }
        return out;
    }

    std::string toJSON();


    //determine all abstarctions bindings
    void determineBinding();

    //apply reduction in normal order to all expressions
    void betaReduceNormalOrder();

    //apply reduction in call by value to all expressions
    void betaReduceCallByValue();


    void readLibrary(std::string);

};

#endif
