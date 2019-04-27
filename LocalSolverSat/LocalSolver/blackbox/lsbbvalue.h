// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXVALUE_H
#define LS_BLACKBOXVALUE_H

#include <climits>
#include <cfloat>
#include <cmath>

namespace localsolverblackbox {
    /**
     * Integer type used in LocalSolverBlackBox model. The range of all integer numbers manipulated in LocalSolverBlackBox model 
     * is limited to {-2^63, ..., 2^63-1}. Note that 2^63 = 9,223,372,036,854,775,808 > 10^18.
     */
    typedef long long lsint;
    
    /** 
     * Float type used in LocalSolverBlackBox model. The precision of all float numbers manipulated in LocalSolverBlackBox model
     * is limited to 64 bits.
     */
    typedef double lsdouble;
}


#endif
