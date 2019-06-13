// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSVALUE_H
#define LS_LSVALUE_H

#include <climits>
#include <cstdlib>
#include <cfloat>
#include <cmath>

namespace localsolver {
    
    /**
     * Integer type used in a %LocalSolver model. The range of all integer 
     * numbers manipulated in a %LocalSolver model is limited to {-2^63, ..., 2^63-1}.
     * Note that 2^63 = 9,223,372,036,854,775,808 > 10^18.
     */
    typedef long long lsint;
    
    /** 
     * Float type used in a %LocalSolver model. The precision of all float
     * numbers manipulated in a %LocalSolver model is limited to 64 bits.
     * 
     * @since 3.0
     */
    typedef double lsdouble;
}

#if defined _WIN32 || defined _WIN64
    #define LS_INT_MAX (_I64_MAX)
    #define LS_INT_MIN (_I64_MIN+1)
    #define LS_INT_NAN (_I64_MIN)

#else
    #define LS_INT_MAX (LLONG_MAX)
    #define LS_INT_MIN (LLONG_MIN+1) 
    #define LS_INT_NAN (LLONG_MIN) 
#endif

#endif
