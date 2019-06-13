// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXSOLUTION_H
#define LS_BLACKBOXSOLUTION_H

#include "lsbbexpression.h"

namespace localsolverblackbox{
    
    /**
     * Solution to the optimization model. A solution carries the values of all expressions 
     * in the model. It is used to retrieve the values of all expressions (getting the solution).
     * 
     * @see LSSolutionStatus
     */
    class LSBBSolution{
    public:
        
        
        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * 
         * @param expr Expression.
         * @see LSBBExpression#isInt()
         * @see LSBBExpression#isBool()
         * @return Value of the expression in this solution.
         */
        lsint getIntValue(const LSBBExpression expr){
            return lsbb_expr_get_int_value(bsolver, expr.exprId);
        }
        
        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is a double.
         * 
         * @param expr Expression.
         * @see LSBBExpression#isDouble()
         * @return Value of the expression in this solution.
         */
        lsdouble getDoubleValue(const LSBBExpression expr){
            return lsbb_expr_get_double_value(bsolver, expr.exprId);
        }
        
        ~LSBBSolution() {}
        LSBBSolution() { bsolver = NULL; }
        LSBBSolution(const LSBBSolution& other) { bsolver = other.bsolver; }
        LSBBSolution& operator=(const LSBBSolution& other) { bsolver = other.bsolver; return *this; }
        
        
    private:
        friend class LocalSolverBlackBox;
        
        explicit LSBBSolution(lsbbsolver* solver) : bsolver(solver) {
            lsbb_check_paused_or_stopped(solver);
        }
        
        lsbbsolver* bsolver;
    };
}

#endif
