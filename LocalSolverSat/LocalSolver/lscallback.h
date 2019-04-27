// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LSCALLBACK_H
#define LSCALLBACK_H

#include "lscallbacktype.h"

namespace localsolver {
    class LocalSolver;

    /** 
     * LSCallback interface. Implementing the LSCallback interface enables users to
     * call functions to react to specific events or to call a function
     * regularly during the search. It can be used for example to control when
     * to stop the search or to display some specific information during the search.
     * 
     * <strong>Note 1:</strong> When a callback is called, the solver is paused.
     * In that state, you can call all the methods marked as \"allowed in state
     * {@link #S_Paused}\". Calling any other method will throw an error.
     * 
     * <strong>Note 2:</strong> %LocalSolver does not manage memory of objects
     * created outside of its environment. Thus, you have to explicitly
     * remove and delete your LSCallback at the end of the search.
     *
     * @since 4.0
     */
    class LSCallback {
    public:
        
        /**
         * The function that will be called during %LocalSolver search.
         *
         * @param solver The solver calling the function.
         * @param type The type of event that triggered the call.
         */
        virtual void callback(LocalSolver& solver, LSCallbackType type) = 0;
        
        /**
         * Default virtual destructor.
         */
        virtual ~LSCallback() {}
    };
}


#endif
