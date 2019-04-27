// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXEXPRESSION_H
#define LS_BLACKBOXEXPRESSION_H

#include "bbentrypoint.h"


namespace localsolverblackbox {

    /**
     * Mathematical modeling expression. Expressions are used to build the mathematical optimization model
     * associated to LocalSolverBlackBox. An expression is composed of an operator (which corresponds to its type) 
     * and its operands (which are other expressions of the model).
     * 
     * @see LSBBModel
     * @see LSBBOperator
     */
    class LSBBExpression {
    public:

        /** 
         * Adds the given operand to this expression.
         * Only allowed in state {@link #S_Modeling}.
         * 
         * @param e Operand to add. 
         */
        void addOperand(LSBBExpression e) {
            lsbb_expr_add_operand(bsolver, exprId, e.exprId);
        }
        
        /** 
         * Returns true if this expression is a boolean (ie 0 or 1), false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return True if the expression is a boolean.
         */
        bool isBool(){
            return lsbb_expr_is_bool(bsolver, exprId);
        }
        
        /** 
         * Returns true if this expression is an integer, false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Note that a boolean is also an integer.
         * 
         * @return True if the expression is an integer.
         */
        bool isInt(){
            return lsbb_expr_is_int(bsolver, exprId);
        }
        
        /** 
         * Returns true if this expression is a double, false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return True if the expression is a double.
         */
        bool isDouble(){
            return lsbb_expr_is_double(bsolver, exprId);
        }

        ~LSBBExpression() {}
        LSBBExpression() { bsolver = NULL; exprId = -1; }
        LSBBExpression(const LSBBExpression& other) { bsolver = other.bsolver; exprId = other.exprId; }
        LSBBExpression operator=(const LSBBExpression& other) { bsolver = other.bsolver; exprId = other.exprId; return *this; }
        
        
    private:
        friend class LSBBModel;
        friend class LSBBSolution;
        
        LSBBExpression(BBSolver* solver, int exprId) : bsolver(solver), exprId(exprId) {
        }
        
        BBSolver* bsolver;
        unsigned int exprId;
    };
}
#endif
