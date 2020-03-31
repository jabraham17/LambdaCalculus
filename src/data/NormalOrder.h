

#ifndef __NORMALORDER_H__
#define __NORMALORDER_H__

#include "Term.h"

namespace NormalOrder {
    Term** findRedex(Term*&);
    //true if reduction could be applied
    void betaReduce(Term*&);
    bool betaReduceStep(Term*&);

}



#endif
