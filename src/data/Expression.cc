

#include "Expression.h"


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
Term** findRedex(Term** term) {

    //possible redexs
    Term** outer = NULL;
    Term** left = NULL;
    Term** right = NULL;


    //if this term is a redex, we found an outer redex
    if((*term)->isBetaRedex()) {
        outer = term;
    }

    //if its an application, check the left and right
    if((*term)->getType() == Term::APP) {
        left = findRedex(&((*term)->application->a));
        right = findRedex(&((*term)->application->b));
    }

    if(outer != NULL) return outer;
    if(outer == NULL && left != NULL) return left;
    if(outer == NULL && left == NULL && right != NULL) return right;

    //otherwise NULL
    return NULL;
}

//apply reduction in normal order
//true if reduction could be applied
void Expression::betaReduceNormalOrder() {
    while(betaReduceNormalOrderStep());
}


bool Expression::betaReduceNormalOrderStep() {
    //get a redex
    Term** redex = findRedex(&this->term);

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