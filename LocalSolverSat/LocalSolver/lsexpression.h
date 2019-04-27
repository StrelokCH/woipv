// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSEXPRESSION_H
#define LS_LSEXPRESSION_H

#include "lsvalue.h"
#include "lsoperator.h"
#include "lsexception.h"
#include "lsnativefunction.h"
#include "lscollection.h"
#include "lsarray.h"
#include "entrypoint.h"
#include <string>

namespace localsolver {

    class LocalSolver;
    class LSModel;

    /**
     * Mathematical modeling expression. Expressions are used to build the mathematical optimization model
     * associated to %LocalSolver. An expression is composed of an operator (which corresponds to its type)
     * and its operands (which are other expressions of the model).
     *
     * @see LSModel
     * @see LSOperator
     */
    class LSExpression {
    public:

        /**
         * Gets the operator of this expression.
         *
         * @return Operator.
         */
        LSOperator getOperator() const {
            return ls_expr_get_operator(bsolver, exprId);
        }

        /**
         * Gets the index of this expression in the model.
         *
         * @return Index in the model.
         */
        int getIndex() const {
            return exprId;
        }

        /**
         * Returns true if this expression is typed as constant
         * in the model, false otherwise.
         *
         * @return True if typed as constant.
         */
        bool isConstant() const {
            return ls_expr_is_constant(bsolver, exprId);
        }

        /**
         * Returns true if this expression is typed as decision
         * in the model, false otherwise.
         *
         * @return True if typed as decision.
         */
        bool isDecision() const {
            return ls_expr_is_decision(bsolver, exprId);
        }

        /**
         * Returns true if this expression is tagged as constraint
         * in the model, false otherwise.
         *
         * @return True if tagged as constraint.
         */
        bool isConstraint() const {
            return ls_expr_is_constraint(bsolver, exprId);
        }

        /**
         * Returns true if this expression is tagged as objective
         * in the model, false otherwise.
         *
         * @return True if tagged as objective.
         */
        bool isObjective() const {
            return ls_expr_is_objective(bsolver, exprId);
        }

        /**
         * Returns true if this expression is a double, false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         *
         * @return True if the expression is a double.
         * @since 3.0
         */
        bool isDouble() const {
            return ls_expr_is_double(bsolver, exprId);
        }

        /**
         * Returns true if this expression is an integer, false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Note that a boolean is also an integer.
         *
         * @return True if the expression is an integer.
         * @since 3.0
         */
        bool isInt() const {
            return ls_expr_is_int(bsolver, exprId);
        }

        /**
         * Returns true if this expression is a boolean (ie 0 or 1), false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         *
         * @return True if the expression is a boolean.
         * @since 3.0
         */
        bool isBool() const {
            return ls_expr_is_bool(bsolver, exprId);
        }

        /**
         * Returns true if this expression is an array, false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         *
         * @return True if the expression is an array.
         * @since 3.1
         */
        bool isArray() const {
            return ls_expr_is_array(bsolver, exprId);
        }

        /**
         * Returns true if this expression is a collection (list or set),
         * false otherwise. Only allowed in states {@link #S_Paused} or
         * {@link #S_Stopped}.
         *
         * @return True if the expression is a collection.
         * @since 5.5
         */
        bool isCollection() const {
            return ls_expr_is_collection(bsolver, exprId);
        }


        /**
         * Returns true if this expression is a function, false otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         *
         * @return True if the expression is a function.
         * @since 6.0
         */
        bool isFunction() const {
            return ls_expr_is_function(bsolver, exprId);
        }

        /**
         * Adds the given operand to this expression.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @param expr Operand to add.
         */
        void addOperand(const LSExpression& expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_expr_add_operand(bsolver, exprId, expr.exprId);
        }

        /**
         * Add the given constant operand to this expression.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @param constant Constant operand to add.
         */
        void addOperand(int constant) {
            ls_expr_add_operand(bsolver, exprId, ls_create_int_constant(bsolver, constant));
        }

        /**
         * Add the given constant operand to this expression.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @param constant Constant operand to add.
         */
        void addOperand(lsint constant) {
            ls_expr_add_operand(bsolver, exprId, ls_create_int_constant(bsolver, constant));
        }

        /**
         * Add the given constant operand to this expression.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @param constant Constant operand to add.
         * @since 3.0
         */
        void addOperand(lsdouble constant) {
            ls_expr_add_operand(bsolver, exprId, ls_create_double_constant(bsolver, constant));
        }


#ifdef LS_CPP11

        /**
         * Add the given operands to this expression.
         * Only allowed in state {@link #S_Modeling}.
         *
         * The operands can be doubles, integers or previously declared LSExpressions.
         * It is also possible to use this method with iterators. In that case,
         * you have to call this method with 2 arguments exactly that must be iterators
         * of the same type, pointing respectively to the initial and final positions of the operands.
         *
         * @tparam TN types of the operands to add. Types allowed: constant types, LSExpression or iterators.
         * @param operands operands.
         */
        template<typename... TN>
        void addOperands(TN... operands) {
            auto dummy = {1, (addOperand(operands), 1)...};
            (void)dummy;
        }

#endif

        /**
         * Add the given operands to this expression.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @param begin iterator to the initial position of the operands to add.
         * @param end iterator to the final position of the operands to add.
         * @since 5.5
         */
        template<typename T0, typename T1>
        void addOperands(T0 begin, T1 end) {
            addOperands(begin, end, is_iterator<T0>(0), is_iterator<T1>(0));
        }

        /**
         * Gets the operand with the given index.
         *
         * @param operandIndex Index of the operand.
         * @return Operand.
         */
        LSExpression getOperand(int operandIndex) const {
            return LSExpression(bsolver, ls_expr_get_operand(bsolver, exprId, operandIndex));
        }

        /**
         * Replaces the operand of the given index.
         *
         * @param operandIndex Index of the operand to change
         * @param operand New operand
         */
        void setOperand(int operandIndex, LSExpression operand) {
            LS_CHECK_SAME_MODEL(bsolver, operand.bsolver);
            ls_expr_set_operand(bsolver, exprId, operandIndex, operand.exprId);
        }

        /**
         * Replaces the operand of the given index.
         *
         * @param operandIndex Index of the operand to change.
         * @param constant New constant operand.
         */
        void setOperand(int operandIndex, lsint constant) {
            ls_expr_set_operand(bsolver, exprId, operandIndex, ls_create_int_constant(bsolver, constant));
        }

        /**
         * Replaces the operand of the given index.
         *
         * @param operandIndex Index of the operand to change.
         * @param constant New constant operand.
         * @since 3.0
         */
        void setOperand(int operandIndex, lsdouble constant) {
            ls_expr_set_operand(bsolver, exprId, operandIndex, ls_create_double_constant(bsolver, constant));
        }

        /**
         * Gets the number of operands of this expression.
         *
         * @return Number of operands.
         */
        int getNbOperands() const {
            return ls_expr_nb_operands(bsolver, exprId);
        }

        /**
         * Sets the value of this expression in the current solution found by the solver.
         * Only allowed for decisions. Only allowed in state {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * If the solver was not launched, this value will be used as an initial value for the decision.
         * This method is a shortcut for {@link LSSolution#setValue(LSExpression, lsint)}
         * and has the same behavior as {@link #setIntValue(lsint)}.
         *
         * @param value Value assigned to this expression.
         * @see LSSolution#setValue(LSExpression, lsint)
         * @see #isInt()
         * @see #isBool()
         */
        void setValue(lsint value) {
            ls_expr_set_int_value(bsolver, exprId, value);
        }

        /**
         * Sets the value of this expression in the current solution found by the solver.
         * Only allowed for decisions. Only allowed in state #S_Stopped.
         * Only allowed if this expression is an integer or a boolean.
         * If the solver was not launched, this value will be used as an initial value for the decision.
         * This method is a shortcut for {@link LSSolution#setValue(LSExpression, lsdouble)}
         * and has the same behavior as {@link #setDoubleValue(lsdouble)}.
         *
         * @param value Value assigned to this expression.
         * @see LSSolution#setValue(LSExpression expr, lsdouble value)
         * @see #isDouble()
         */
        void setValue(lsdouble value) {
            ls_expr_set_double_value(bsolver, exprId, value);
        }

        /**
         * Sets the value of this expression in the current solution found by the solver.
         * Only allowed for decisions. Only allowed in state {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * If the solver was not launched, this value will be used as an initial value for the decision.
         * This method is a shortcut for {@link LSSolution#setValue(LSExpression, lsint)}.
         *
         * @param value Value assigned to this expression.
         * @see LSSolution#setIntValue(LSExpression, lsint)
         * @see #isInt()
         * @see #isBool()
         * @since 3.0
         */
        void setIntValue(lsint value) {
            ls_expr_set_int_value(bsolver, exprId, value);
        }

        /**
         * Sets the value of this expression in the current solution found by the solver.
         * Only allowed for decisions. Only allowed in state {@link #S_Stopped}.
         * Only allowed if this expression is a double.
         * If the solver was not launched, this value will be used as an initial value for the decision.
         * This method is a shortcut for {@link LSSolution#setValue(LSExpression, lsdouble)}.
         *
         * @param value Value assigned to this expression.
         * @see LSSolution#setDoubleValue(LSExpression, lsdouble)
         * @see #isDouble()
         * @since 3.0
         */
        void setDoubleValue(lsdouble value) {
            ls_expr_set_double_value(bsolver, exprId, value);
        }

        /**
         * Gets the value of this expression in the best solution found by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * This method is a shortcut for {@link LSSolution#getValue(LSExpression)}.
         *
         * @return Value in the best solution.
         * @see LSSolution#getValue(LSExpression)
         * @see #isInt()
         * @see #isBool()
         */
        lsint getValue() const {
            return ls_expr_get_int_value(bsolver, exprId);
        }

        /**
         * Gets the value of this expression in the best solution found by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an integer or a boolean.
         * This method is a shortcut for {@link LSSolution#getIntValue(LSExpression)}.
         *
         * @return Value in the best solution.
         * @see LSSolution#getIntValue(LSExpression)
         * @see #isInt()
         * @see #isBool()
         * @since 3.0
         */
        lsint getIntValue() const {
            return ls_expr_get_int_value(bsolver, exprId);
        }

        /**
         * Gets the value of this expression in the best solution found by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is a double.
         * This method is a shortcut for {@link LSSolution#getDoubleValue(LSExpression)}.
         *
         * @return Value in the best solution.
         * @see LSSolution#getDoubleValue(LSExpression)
         * @see #isDouble()
         * @since 3.0
         */
        lsdouble getDoubleValue() const {
            return ls_expr_get_double_value(bsolver, exprId);
        }

        /**
         * Gets the value of this expression in the best solution found by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is a collection (list or set).
         * This method is a shortcut for {@link LSSolution#getCollectionValue(LSExpression)}.
         *
         * @return Value in the best solution.
         * @see LSExpression#isCollection
         * @see LSSolution#getCollectionValue(LSExpression)
         * @since 5.5
         */
        LSCollection getCollectionValue() const {
            return LSCollection(bsolver, exprId);
        }
        
        /**
         * Gets the value of this expression in the best solution found by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Only allowed if this expression is an array.
         * This method is a shortcut for {@link LSSolution#getArrayValue(LSExpression)}.
         *
         * @return Value in the best solution.
         * @see LSExpression#isArray
         * @see LSSolution#getArrayValue(LSExpression)
         * @since 7.5
         */
        LSArray getArrayValue() const {
            return LSArray(bsolver, ls_expr_get_array_value(bsolver, exprId));
        }

        /**
         * Returns true if the given expression is violated in the best solution found by the solver.
         * An expression can be violated in 3 cases:
         *    1. it is a constraint and its value is 0
         *    2. it is a a double objective and its value is NaN (NotANumber)
         *    3. it is a constraint with no valid value (arithmetic or out of bounds exception or NaN operands).
         * Note that only constraints and objectives can be violated. Other expression can have
         * undefined value provided that it does not impact a constraint or objective.
         * 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * This method is a shortcut for {@link LSSolution#isViolated(LSExpression)}.
         *
         * @return True if this expression is violated in the best solution.
         * @since 5.5
         * @see #isUndefined()
         */
        bool isViolated() const {
            return ls_expr_is_violated(bsolver, exprId);
        }

        /**
         * Returns true if the given expression has an undefined value in the best solution found by the solver.
         * An expression can be undefined in 2 cases:
         *    1. it is a a double and its value is NaN (NotANumber)
         *    2. it is an integer or boolean with no valid value (arithmetic or out of bounds exception).
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * This method is a shortcut for {@link LSSolution#isUndefined(LSExpression)}.
         *
         * @return True if this expression has an undefined value in the best solution.
         * @since 7.0
         */
        bool isUndefined() const {
            return ls_expr_is_undefined(bsolver, exprId);
        }

        /**
         * Sets the name of this expression. Only allowed in state {@link #S_Modeling}.
         * The name cannot be empty. Two operators of the model cannot share the
         * same name. Useful for debugging or logging purposes.
         *
         * @param name Name.
         */
        void setName(const std::string& name) {
            ls_expr_set_name(bsolver, exprId, name.c_str());
        }

        /**
         * Returns true if this expression has a name, and false otherwise.
         *
         * @return True if named.
         */
        bool isNamed() const {
            return ls_expr_is_named(bsolver, exprId);
        }

        /**
         * Gets the name of this expression or the empty string if no name has been set.
         *
         * @return Name.
         */
        const std::string getName() const {
            int size = ls_expr_get_name(bsolver, exprId, NULL, 0);
            std::string str(size, '\0');
            ls_expr_get_name(bsolver, exprId, &str[0], size);
            return str;
        }

        /**
         * Returns a string representation of this expression. This representation provides
         * the index of the expression, its type, and its name (if any). Useful for debugging
         * or logging purposes.
         *
         * @return String representation.
         */
        std::string toString() const {
            int size = ls_expr_to_string(bsolver, exprId, NULL, 0);
            std::string str(size, '\0');
            ls_expr_to_string(bsolver, exprId, &str[0], size);
            return str;
        }

        /**
         * Returns useful info about this expression (according to the state of LocalSolver).
         * Useful for debugging or logging purposes.
         *
         * @return Info about this search during the solving process.
         */
        std::string getInfo() const {
            int size = ls_expr_info(bsolver, exprId, NULL, 0);
            std::string str(size, '\0');
            ls_expr_info(bsolver, exprId, &str[0], size);
            return str;
        }

        /**
         * Creates a new {@link #O_Sum} expression.
         * It is a shortcut for ``model.createExpression(O_Sum, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Sum} expression.
         */
        template<typename T>
        LSExpression operator+(T operand) { return createExpressionRight(O_Sum, operand); }
        
        /**
         * Creates a new {@link #O_Sub} expression.
         * It is a shortcut for ``model.createExpression(O_Sub, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Sub} expression.
         */
        template<typename T>
        LSExpression operator-(T operand) { return createExpressionRight(O_Sub, operand); }
        
        /**
         * Creates a new {@link #O_Prod} expression.
         * It is a shortcut for ``model.createExpression(O_Prod, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Prod} expression.
         */
        template<typename T>
        LSExpression operator*(T operand) { return createExpressionRight(O_Prod, operand); }
        
        /**
         * Creates a new {@link #O_Mod} expression.
         * It is a shortcut for ``model.createExpression(O_Mod, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression or an integer.
         * @return A new {@link #O_Mod} expression.
         */
        template<typename T>
        LSExpression operator%(T operand) { return createExpressionRight(O_Mod, operand); }
        
        /**
         * Creates a new {@link #O_Div} expression.
         * It is a shortcut for ``model.createExpression(O_Div, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Div} expression.
         */
        template<typename T>
        LSExpression operator/(T operand) { return createExpressionRight(O_Div, operand); }

        /**
         * Creates a new {@link #O_Not} expression.
         * It is a shortcut for ``model.createExpression(O_Not, this)``.
         * 
         * @return A new {@link #O_Not} expression.
         */
        LSExpression operator!() { return createUnaryExpression(O_Not); }
        
        /**
         * Creates a new {@link #O_And} expression.
         * It is a shortcut for ``model.createExpression(O_And, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression or a boolean.
         * @return A new {@link #O_And} expression.
         */
        template<typename T>
        LSExpression operator&&(T operand) { return createExpressionRight(O_And, operand); }
        
        /**
         * Creates a new {@link #O_Or} expression.
         * It is a shortcut for ``model.createExpression(O_Or, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression or a boolean.
         * @return A new {@link #O_Or} expression.
         */
        template<typename T>
        LSExpression operator||(T operand) { return createExpressionRight(O_Or, operand); }
        
        /**
         * Creates a new {@link #O_Xor} expression.
         * It is a shortcut for ``model.createExpression(O_Xor, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression or a boolean.
         * @return A new {@link #O_Xor} expression.
         */
        template<typename T>
        LSExpression operator^(T operand) { return createExpressionRight(O_Xor, operand); }

        /**
         * Creates a new {@link #O_Eq} expression.
         * It is a shortcut for ``model.createExpression(O_Eq, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Eq} expression.
         */
        template<typename T>
        LSExpression operator==(T operand) { return createExpressionRight(O_Eq, operand); }
        
        /**
         * Creates a new {@link #O_Neq} expression.
         * It is a shortcut for ``model.createExpression(O_Neq, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Neq} expression.
         */
        template<typename T>
        LSExpression operator!=(T operand) { return createExpressionRight(O_Neq, operand); }
        
        /**
         * Creates a new {@link #O_Geq} expression.
         * It is a shortcut for ``model.createExpression(O_Geq, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Geq} expression.
         */
        template<typename T>
        LSExpression operator>=(T operand) { return createExpressionRight(O_Geq, operand); }
        
        /**
         * Creates a new {@link #O_Leq} expression.
         * It is a shortcut for ``model.createExpression(O_Leq, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Leq} expression.
         */
        template<typename T>
        LSExpression operator<=(T operand) { return createExpressionRight(O_Leq, operand); }
        
        /**
         * Creates a new {@link #O_Gt} expression.
         * It is a shortcut for ``model.createExpression(O_Gt, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Gt} expression.
         */
        template<typename T>
        LSExpression operator>(T operand) { return createExpressionRight(O_Gt, operand); }
        
        /**
         * Creates a new {@link #O_Lt} expression.
         * It is a shortcut for ``model.createExpression(O_Lt, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_Lt} expression.
         */
        template<typename T>
        LSExpression operator<(T operand) { return createExpressionRight(O_Lt, operand); }

        /**
         * Creates a new {@link #O_At} expression.
         * It is a shortcut for ``model.createExpression(O_At, this, operand)``.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return A new {@link #O_At} expression.
         */
        template<typename T>
        LSExpression operator[](T operand) { return createExpressionRight(O_At, operand); }

        /**
         * Sums the given operand with the current expression. If the current expression
         * is already of type {@link #O_Sum}, the given operand is simply pushed on the current list
         * of operands. A new {@link #O_Sum} expression is created otherwise.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return Return the current expression or a new {@link #O_Sum} expression.
         */
        template<typename T>
        LSExpression operator+=(T operand) { return addCompoundOperand(operand, O_Sum); }
        
        /**
         * Multiply the given operand with the current expression. If the current expression
         * is already of type {@link #O_Prod}, the given operand is simply pushed on the current
         * list of operands. A new {@link #O_Prod} expression is created otherwise.
         * 
         * @param operand Operand. Can be an LSExpression, an integer or a double.
         * @return Return the current expression or a new {@link #O_Prod} expression.
         */
        template<typename T>
        LSExpression operator*=(T operand) { return addCompoundOperand(operand, O_Prod); }
        
        /**
         * Creates a logical AND between the current expression and the given operand. If the
         * current expression is already of type {@link #O_And}, the given operand is simply pushed
         * on the current list of operands. A new {@link #O_And} expression is created otherwise.
         * 
         * @param operand Operand. Can be an LSExpression or a boolean.
         * @return Return the current expression or a new {@link #O_And} expression.
         */
        template<typename T>
        LSExpression operator&=(T operand) { return addCompoundOperand(operand, O_And); }

        /**
         * Creates a logical XOR between the current expression and the given operand. If the
         * current expression is already of type {@link #O_Xor}, the given operand is simply pushed
         * on the current list of operands. A new {@link #O_Xor} expression is created otherwise.
         * 
         * @param operand Operand. Can be an LSExpression or a boolean.
         * @return Return the current expression or a new {@link #O_Xor} expression.
         */        
        template<typename T>
        LSExpression operator^=(T operand) { return addCompoundOperand(operand, O_Xor); }
        
        /**
         * Creates a logical OR between the current expression and the given operand. If the
         * current expression is already of type {@link #O_Or}, the given operand is simply pushed
         * on the current list of operands. A new {@link #O_Or} expression is created otherwise.
         * 
         * @param operand Operand. Can be an LSExpression or a boolean.
         * @return Return the current expression or a new {@link #O_Or} expression.
         */
        template<typename T>
        LSExpression operator|=(T operand) { return addCompoundOperand(operand, O_Or); }


#ifdef LS_CPP11

        /**
         * Creates a {@link #O_Call} expression with the given operands as arguments.
         * It is a shortcut for ``model.createExpression(O_Call, this, operands)``.
         * 
         * @tparam TN types of the operands to add. Types allowed: constant types, LSExpression or iterators.
         * @param operands operands.
         */ 
        template<typename... TN>
        LSExpression operator()(TN... operands) { return createExpressionRight(O_Call, operands...); }

#else

        template<typename T>
        LSExpression operator()(T operand) { return createExpressionRight(O_Call, operand); }
        template<typename T1, typename T2>
        LSExpression operator()(T1 op1, T2 op2) { return createExpressionRight(O_Call, op1, op2); }

#endif

        ~LSExpression() {}
        LSExpression() { bsolver = NULL; exprId = -1; }
        LSExpression(const LSExpression& other) { bsolver = other.bsolver; exprId = other.exprId; }
        LSExpression operator=(const LSExpression& other) { bsolver = other.bsolver; exprId = other.exprId; return *this; }

    private:
        friend class LocalSolver;
        friend class LSModel;
        friend class LSSolution;
        friend class LSInconsistency;

        template<typename T>
        friend typename LSCompatibleArg<T>::type operator+(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator-(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator*(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator/(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator%(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator==(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator!=(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator>=(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator<=(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator>(T expr0, LSExpression expr1);
        template<typename T>
        friend typename LSCompatibleArg<T>::type operator<(T expr0, LSExpression expr1);

        LSExpression(lssolver* solver, int exprId) : bsolver(solver), exprId(exprId) {
            ls_check_not_running(bsolver);
            ls_expr_check_index(bsolver, exprId);
        }

        lssolver* bsolver;
        unsigned int exprId;

        template<typename T0, typename T1>
        void addOperands(T0 expr0, T1 expr1, without_iterator, without_iterator) {
            addOperand(expr0);
            addOperand(expr1);
        }

        template<typename T>
        void addOperands(T begin, T end, with_iterator, with_iterator) {
            for(T it = begin; it != end; ++it) {
                addOperand(*it);
            }
        }

        template<typename T>
        LSExpression addCompoundOperand(T operand, LSOperator op) {
            if(ls_expr_get_operator(bsolver, exprId) == op) {
                addOperand(operand);
                return *this;
            } else {
                LSExpression newExpr(bsolver, ls_create_expression(bsolver, op));
                newExpr.addOperand(*this);
                newExpr.addOperand(operand);
                return newExpr;
            }
        }

        LSExpression createUnaryExpression(LSOperator op) {
            LSExpression newExpr(bsolver, ls_create_expression(bsolver, op));
            newExpr.addOperand(*this);
            return newExpr;
        }

#ifdef LS_CPP11

        template<typename... TN>
        LSExpression createExpressionRight(LSOperator op, TN... operands) {
            LSExpression newExpr(bsolver, ls_create_expression(bsolver, op));
            newExpr.addOperand(*this);
            newExpr.addOperands(operands...);
            return newExpr;
        }

#else

        template<typename T1>
        LSExpression createExpressionRight(LSOperator op, T1 expr1) {
            LSExpression newExpr(bsolver, ls_create_expression(bsolver, op));
            newExpr.addOperand(*this);
            newExpr.addOperand(expr1);
            return newExpr;
        }

        template<typename T1, typename T2>
        LSExpression createExpressionRight(LSOperator op, T1 expr1, T2 expr2) {
            LSExpression newExpr(bsolver, ls_create_expression(bsolver, op));
            newExpr.addOperand(*this);
            newExpr.addOperands(expr1, expr2);
            return newExpr;
        }

#endif

        template<typename T0>
        LSExpression createExpressionLeft(LSOperator op, T0 expr0) {
            LSExpression newExpr(bsolver, ls_create_expression(bsolver, op));
            newExpr.addOperand(expr0);
            newExpr.addOperand(*this);
            return newExpr;
        }
    };

    template<typename T>
    inline typename LSCompatibleArg<T>::type operator+(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Sum, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator-(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Sub, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator*(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Prod, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator/(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Div, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator%(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Mod, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator==(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Eq, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator!=(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Neq, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator>=(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Geq, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator<=(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Leq, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator>(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Gt, expr0); }
    template<typename T>
    inline typename LSCompatibleArg<T>::type operator<(T expr0, LSExpression expr1) { return expr1.createExpressionLeft(O_Lt, expr0); }
}

#endif