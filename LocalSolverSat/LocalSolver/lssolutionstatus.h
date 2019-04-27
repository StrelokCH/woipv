// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSSOLUTIONSTATUS_H
#define LS_LSSOLUTIONSTATUS_H

namespace localsolver {

    /** 
     * Solution status: Optimal, Feasible, Infeasible or Inconsistent. 
     * 
     * @see LSSolution
     */
    enum LSSolutionStatus {
        /**
         * Solution and model are inconsistent.
         * The solver was able to prove that the model admits no feasible solution. 
         * Note that even a model without any constraint can be inconsistent, because some
         * computations can yield undefined results. For instance, computing sqrt(x)
         * with negative x yields an undefined value, which causes the solution to be
         * invalid if it is used in an objective or a constraint (directly or indirectly).
         */
        SS_Inconsistent,

        /** 
         * Solution is infeasible (some constraints are violated).          
         */
        SS_Infeasible,

        /** 
         * Solution is feasible but optimality was not proven. 
         */
        SS_Feasible,

        /** 
         * Solution is optimal (all objective bounds are reached). 
         */
        SS_Optimal
    };
}

#endif
