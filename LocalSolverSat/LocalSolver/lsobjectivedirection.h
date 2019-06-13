// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSOBJECTIVEDIRECTION_H
#define LS_LSOBJECTIVEDIRECTION_H

namespace localsolver {

    /** 
     * Objective directions. 
     * 
     * @see LSModel#addObjective(LSExpression*, LSObjectiveDirection)
     */
    enum LSObjectiveDirection {
        /**
         * Minimization.
         */
        OD_Minimize,

        /**
         * Maximization.
         */
        OD_Maximize
    };
}

#endif
