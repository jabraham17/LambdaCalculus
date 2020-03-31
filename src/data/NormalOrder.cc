//
// Created by Jacob Abraham on 3/30/20.
//

#include "NormalOrder.h"



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
Term** NormalOrder::findRedex(Term*& term) {

    //possible redexs
    Term** outer = NULL;
    Term** left = NULL;
    Term** right = NULL;

    //if this term is a redex, we found an outer redex
    if(term->isBetaRedex()) {
        outer = &term;
    }

    //if its an application, check the left and right
    if(term->isType(APPLICATION)) {
        left = findRedex(term->getTermA());
        right = findRedex(term->getTermB());
    }
    //if its an abstraction, check the body
    else if(term->isType(ABSTRACTION)) {
        return findRedex(term->getBody());
    }

    Term** ret = NULL;
    if(outer != NULL) ret = outer;
    else if(outer == NULL && left != NULL) ret = left;
    else if(outer == NULL && left == NULL && right != NULL) ret = right;


    return ret;
}

#include <iostream>
void NormalOrder::betaReduce(Term*& term) {
    while(betaReduceStep(term)) {
        std::cout << *term << std::endl;
    }
}

//tru if reduction could be applied
bool NormalOrder::betaReduceStep(Term*& term) {
    //get a redex
    Term** redex = findRedex(term);

    //if its NULL, no more redexs
    if(redex == NULL) return false;

    //eval the redex
    applyBetaRedex(*redex);

    return true;
}
