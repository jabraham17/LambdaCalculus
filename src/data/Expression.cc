

#include "Expression.h"
#include <iostream>

Term* findDefine(Name* name, std::vector<Define*> defines) {
    //check the defines for a define
    for(auto d: defines) {
        if(*(d->getName()) == *name) {

            //make a copy of the term
            Term* old = d->getExpression()->term;
            Term* newTerm = new Term(*old);

            return newTerm;
        }
    }
    return NULL;
}



/*
 * Truth table for how these redexs return
 * return the outermost leftmost redex
 *
 * O L R  Return
 * ==================
 * F F F  NULL
 * F F T  right
 * F T F  left
 * F T T  left
 * T F F  outer
 * T F T  outer
 * T T F  outer
 * T T T  outer
 *
 * O L R  Return
 * ==================
 * F F F  NULL
 * F F T  right
 * F T X  left
 * T X X  outer
 *
 */
//find the leftmost outermost redex
Term** findRedex(Term** term, std::vector<Define*> defines) {

    //possible redexs
    Term** outer = NULL;
    Term** left = NULL;
    Term** right = NULL;


    //if this term is a redex, we found an outer redex
    if((*term)->isBetaRedex()) {
        //if the left is a definition, expand it
        if((*term)->application->a->isDefinition()) {
            Name* name = (*term)->application->a->atom->name;
            Term* replacement = findDefine(name, defines);
            if(replacement == NULL) {
                std::cout << "Error:  " << name->getName() << " is not defined" << std::endl;
                exit(1);
            }
            (*term)->application->a = replacement;
            //we replace the term and recall this function
            return findRedex(term, defines);
        }
        else outer = term;
    }

    //if its an application, check the left and right
    if((*term)->getType() == Term::APP) {
        left = findRedex(&((*term)->application->a), defines);
        right = findRedex(&((*term)->application->b), defines);
    }

    if(outer != NULL) return outer;
    if(outer == NULL && left != NULL) return left;
    if(outer == NULL && left == NULL && right != NULL) return right;

    //otherwise NULL
    return NULL;
}

//apply reduction in normal order
//true if reduction could be applied
void Expression::betaReduceNormalOrder(std::vector<Define*> defines) {
    while(betaReduceNormalOrderStep(defines));
}


bool Expression::betaReduceNormalOrderStep(std::vector<Define*> defines) {
    //get a redex
    Term** redex = findRedex(&this->term, defines);

    //if its NULL, no more redexs
    if(redex == NULL) return false;

    //eval the redex
    applyBetaRedex(*redex);

    return true;
}


void findRedexsNotInAbstraction(Term** term, std::vector<Term**>& redexs) {
    //not a abstraction and is a redex, add it
    if((*term)->getType() != Term::ABS && (*term)->isBetaRedex()) {
        redexs.push_back(term);
    }
        //if its an application, could be a redex in a or b, check it
    else if((*term)->getType() == Term::APP) {
        findRedexsNotInAbstraction(&((*term)->application->a), redexs);
        findRedexsNotInAbstraction(&((*term)->application->b), redexs);
    }
}

//apply reduction in call by value
void Expression::betaReduceCallByValue() {
    while(betaReduceCallByValueStep());
}

//apply reduction in call by value
//true if reduction could be applied
bool Expression::betaReduceCallByValueStep() {
    //get a list of all terms that are redexs
    //these redexs must not be in an abstraction
    std::vector<Term**> redexs;
    findRedexsNotInAbstraction(&(this->term), redexs);

    //a redex is an application, meaning it has an 'a' and 'b' term
    //filter the redexs, we only want the ones in which 'b' is a value
    std::vector<Term**> filteredRedexs;
    for(auto redex: redexs) {
        if((*redex)->application->b->isValue()) {
            filteredRedexs.push_back(redex);
        }
    }
    redexs = filteredRedexs;


    //if there are redexs, pop off one and eval it
    if(redexs.size() >= 1) {
        Term** redex = redexs.back();

        applyBetaRedex(*redex);

        //maybe more redexs, possibly more
        return true;
    }
    //no more redexs
    else return false;
}

std::ostream& operator<<(std::ostream& out, const Expression& e) {

    out << *(e.term);

    return out;
}