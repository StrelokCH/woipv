// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSSTATE_H
#define LS_LSSTATE_H

namespace localsolver {

    /** 
     * State of LocalSolver environment.
     * 
     * @see LocalSolver#getState() 
     */
    enum LSState {
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
