// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSSOLUTION_H
#define LS_LSSOLUTION_H

#include "lsvalue.h"
#include "lsexpression.h"
#include "lsexception.h"
#include "entrypoint.h"

namespace localsolver {
    class LocalSolver;

    /**
     * Solution to the optimization model. A solution carries the values of all expressions 
     * in the model. It is used to retrieve the values of all expressions (getting the solution), or to set 
     * the values of some decisions (setting the solution).
     * 
     * @see LSSolutionStatus
     */
    class LSSolution {
    public:

        /**
         * Sets the value of the given expression in this solution. Only decisions 
         * can be set. Only allowed in state {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * If the solver was not launched, this value will be used as an initial value for the decision.
         * This method has the same behavior as {@link #setIntValue}.
         * 
         * @param expr Decision.
         * @param value Value assigned to the decision in this solution.
         * @see LSExpression#isDecision()
         * @see LSExpression#isInt()
         * @see LSExpression#isBool()
         */
        void setValue(LSExpression expr, lsint value) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_expr_set_int_value(bsolver, expr.exprId, value);
        }
        
        /**
         * Sets the value of the given expression in this solution. Only decisions 
         * can be set. Only allowed in state {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is a double.
         * If the solver was not launched, this value will be used as an initial value for the decision.
         * This method has the same behavior as {@link #setDoubleValue}.
         * 
         * @param expr Decision.
         * @param value Value assigned to the decision in this solution.
         * @see LSExpression#isDecision()
         * @see LSExpression#isDouble()
         */
        void setValue(LSExpression expr, lsdouble value) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_expr_set_double_value(bsolver, expr.exprId, value);
        }

        /**
         * Sets the value of the given expression in this solution. Only decisions 
         * can be set. Only allowed in state {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * If the solver was not launched, this value will be used as an initial
         * value for the decision.
         * 
         * @param expr Decision.
         * @param value Value assigned to the decision in this solution.
         * @see LSExpression#isDecision()
         * @see LSExpression#isInt()
         * @since 3.0
         */
        void setIntValue(LSExpression expr, lsint value) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_expr_set_int_value(bsolver, expr.exprId, value);
        }

        /**
         * Sets the value of the given expression in this solution. Only decisions 
         * can be set. Only allowed in state {@link #S_Paused} or {@link #S_Stopped}.
         * If the solver was not launched, this value will be used as an initial
         * value for the decision.
         * Only allowed if this expression is a double.
         * 
         * @param expr Decision.
         * @param value Value assigned to the decision in this solution.
         * @see LSExpression#isDecision()
         * @see LSExpression#isDouble()
         * @since 3.0
         */
        void setDoubleValue(LSExpression expr, lsdouble value) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_expr_set_double_value(bsolver, expr.exprId, value);
        }

        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * 
         * @param expr Expression.
         * @see LSExpression#isInt()
         * @see LSExpression#isBool()
         * @return Value of the expression in this solution.
         */
        lsint getValue(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            return ls_expr_get_int_value(bsolver, expr.exprId);
        }

        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is a double.
         * 
         * @param expr Expression.
         * @see LSExpression#isDouble()
         * @return Value of the expression in this solution.
         * @since 3.0
         */
        lsdouble getDoubleValue(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            return ls_expr_get_double_value(bsolver, expr.exprId);
        }

        
        /**
         * Returns true if the given expression is violated in this solution.
         * An expression can be violated in 3 cases:
         *    1. it is a constraint and its value is 0
         *    2. it is a a double objective and its value is NaN (NotANumber)
         *    3. it is a constraint with no valid value (arithmetic or out of bounds exception or NaN operands).
         * Note that only constraints and objectives can be violated. Other expression can have
         * undefined value provided that it does not impact a constraint or objective.
         * 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param expr Expression.
         * @return True if this expression is violated in this solution.
         * @since 5.5
         * @see #isUndefined()
         */
        bool isViolated(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            return ls_expr_is_violated(bsolver, expr.exprId);
        }
        
        /**
         * Returns true if the given expression has an undefined value in this solution. 
         * An expression can be undefined in 2 cases:
         *    1. it is a a double and its value is NaN (NotANumber)
         *    2. it is an integer or boolean with no valid value (arithmetic or out of bounds exception).
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param expr Expression.
         * @return True if this expression has an undefined value in this solution. 
         * @since 7.0
         */
        bool isUndefined(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            return ls_expr_is_undefined(bsolver, expr.exprId);
        }

        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * 
         * @param expr Expression.
         * @see LSExpression#isInt()
         * @see LSExpression#isBool()
         * @return Value of the expression in this solution.
         * @since 3.0
         */
        lsint getIntValue(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            return ls_expr_get_int_value(bsolver, expr.exprId);
        }

        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is a collection (list or set).
         * 
         * @param expr Expression.
         * @see LSExpression#isCollection()
         * @return Value of the expression in this solution.
         * @since 5.5
         */
        LSCollection getCollectionValue(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            if (!expr.isCollection())
                throw LSException(EC_Api, __FILE__, __LINE__, __FUNCTION__, "This expression is not a collection."); \
            return LSCollection(bsolver, expr.exprId);
        }
        
        /**
         * Returns the value of the given expression in this solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an array.
         * 
         * @param expr Expression.
         * @see LSExpression#isArray()
         * @return Value of the expression in this solution.
         * @since 7.5
         */
        LSArray getArrayValue(const LSExpression expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            return LSArray(bsolver, ls_expr_get_array_value(bsolver, expr.exprId));
        }
        
        /**
         * Returns the status of the solution.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Status of the solution.
         * @see LSSolutionStatus
         */
        LSSolutionStatus getStatus() {
            return ls_get_solution_status(bsolver);
        }

        /**
         * Clear the solution and set all decisions to zero.
         * Only allowed in state {@link #S_Stopped}.
         */
        void clear() {
            ls_clear_solution(bsolver);
        }

        ~LSSolution() {}
        LSSolution() { bsolver = NULL; }
        LSSolution(const LSSolution& other) { bsolver = other.bsolver; }
        LSSolution& operator=(const LSSolution& other) { bsolver = other.bsolver; return *this; }
        
    private:
        friend class LocalSolver;

        explicit LSSolution(lssolver* solver) : bsolver(solver) {
            ls_check_paused_or_stopped(bsolver);
        }

        lssolver* bsolver;
    };
}

#endif