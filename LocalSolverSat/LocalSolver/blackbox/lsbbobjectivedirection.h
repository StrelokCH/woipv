// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXOBJECTIVEDIRECTION_H
#define LS_BLACKBOXOBJECTIVEDIRECTION_H

namespace localsolverblackbox {

    /** 
     * Objective directions. 
     * 
     * @see LSBBModel#addObjective(LSBBExpression*, LSBBObjectiveDirection)
     */
    enum LSBBObjectiveDirection {
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
