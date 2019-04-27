// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXSTATE_H
#define LS_BLACKBOXSTATE_H

namespace localsolverblackbox {

    /** 
     * State of LocalSolverBlackBox environment.
     * 
     * @see LocalSolverBlackBox#getState() 
     */
    enum LSBBState {
        /** 
         * Model is being built. 
         */
        S_Modeling,

        /** 
         * Solver is running. 
         */
        S_Running,

        /** 
         * Solver is paused. 
         */
        S_Paused,

        /** 
         * Solver is stopped. 
         */
        S_Stopped
    };
}

#endif
