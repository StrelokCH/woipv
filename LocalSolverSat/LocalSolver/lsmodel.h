// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSMODEL_H
#define LS_LSMODEL_H

#include "lsvalue.h"
#include "lsexpression.h"
#include "lsoperator.h"
#include "lsexception.h"
#include "lsnativefunction.h"
#include "entrypoint.h"
#include <string>
#include <vector>

#ifdef LS_CPP11
#include <functional>
#include <array>
#endif

namespace localsolver {
    class LocalSolver;

    /**
     * Mathematical optimization model. A model is composed of expressions (some of which are decisions),
     * organized as a tree. Then, some expressions of the model can be constrained or optimized.
     * Once your optimization model is created and closed, the solver can be launched to resolve it.
     * Note that you cannot modify a model which has been closed: you must reopen it (with {@link #open()})
     * or instantiate another LocalSolver environment to optimize another model.
     *
     * @see LSExpression
     * @see LSOperator
     */
    class LSModel {
    public:

        /**
         * Creates a constant expression representing the given value.
         * Only allowed in state {@link #S_Modeling}. Note that if a constant has been
         * already created with the same value, this method can return the same expression,
         * but it is not guaranteed. The exact behavior is implementation defined.
         *
         * @param value Value of the constant.
         * @return Created constant expression.
         */
        LSExpression createConstant(lsint value) {
            return LSExpression(bsolver, ls_create_int_constant(bsolver, value));
        }

        /**
         * Creates a constant expression representing the given value.
         * Only allowed in state {@link #S_Modeling}. Note that if a constant has been
         * already created with the same value, this method can return the same expression,
         * but it is not guaranteed. The exact behavior is implementation defined.
         *
         * @param value Value of the constant
         * @return Created constant expression.
         */
        LSExpression createConstant(lsdouble value) {
            return LSExpression(bsolver, ls_create_double_constant(bsolver, value));
        }

#ifdef LS_CPP11

        /**
         * Creates an expression of the given type, with the given ordered operands.
         * Only allowed in state {@link #S_Modeling}. The operands can be doubles, integers
         * or previously declared LSExpressions. It is also possible to use this
         * method with iterators. In that case, you have to call this method with
         * 2 operands exactly that must be iterators of the same type, pointing
         * respectively to the initial and final positions of the operands.
         *
         * @tparam TN types of the operands to add. Types allowed: constant types, LSExpression or iterators.
         * @param op Type of expression to create.
         * @param operands Operands.
         * @return Created expression.
         */
        template<typename... TN>
        LSExpression createExpression(LSOperator op, TN... operands) {
            LSExpression expr(bsolver, ls_create_expression(bsolver, op));
            expr.addOperands(operands...);
            return expr;
        }

#else

        /**
         * Creates an expression of the given type. The expression is created without operands.
         * Only allowed in state {@link #S_Modeling}. This method cannot be used to create constants:
         * use {@link #createConstant(lsint)} or {@link #createConstant(lsdouble)} instead.
         * Once the expression created, you can add operands to it with {@link LSExpression#addOperands}.
         *
         * @param op Type of expression to create.
         * @return Created expression.
         */
        LSExpression createExpression(LSOperator op) {
            return LSExpression(bsolver, ls_create_expression(bsolver, op));
        }

        /**
         * Creates an expression of the given type, with the given operand.
         * Only allowed in state {@link #S_Modeling}. Useful for creating unary expressions.
         * The operand can be a double, an integer or a previously declared LSExpression.
         *
         * @tparam T0 type of the operand to add. Types allowed: constant types or LSExpression.
         * @param op Type of expression to create.
         * @param expr0 Operand 0.
         * @return Created expression.
         */
        template<typename T0>
        LSExpression createExpression(LSOperator op, T0 expr0) {
            LSExpression expr(bsolver, ls_create_expression(bsolver, op));
            expr.addOperand(expr0);
            return expr;
        }

        /**
         * Creates an expression of the given type, with the given ordered operands.
         * Only allowed in state #S_Modeling. Useful for creating binary expressions.
         * The operands can be doubles, integers or previously declared LSExpressions.
         * It is also possible to use this method with iterators. In that case,
         * expr0 and expr1 must be iterators of the same type, pointing respectively
         * to the initial and final positions of the operands
         *
         * @tparam T0 type of the operand to add. Types allowed: constant types, LSExpression or iterator.
         * @tparam T1 type of the operand to add. Types allowed: constant types, LSExpression or iterator.
         * @param op Type of expression to create.
         * @param expr0 Operand 0.
         * @param expr1 Operand 1.
         * @return Created expression.
         */
        template<typename T0, typename T1>
        LSExpression createExpression(LSOperator op, T0 expr0, T1 expr1) {
            LSExpression expr(bsolver, ls_create_expression(bsolver, op));
            expr.addOperands(expr0, expr1);
            return expr;
        }

        /**
         * Creates an expression of the given type, with the given ordered operands.
         * Only allowed in state {@link #S_Modeling}. Useful for creating ternary expressions
         * (in particular, #O_If expressions). The operands can be doubles, integers
         * or previously declared LSExpressions.
         *
         * @tparam T0 type of the operand to add. Types allowed: constant types or LSExpression.
         * @tparam T1 type of the operand to add. Types allowed: constant types or LSExpression.
         * @tparam T2 type of the operand to add. Types allowed: constant types or LSExpression.
         * @param op Type of expression to create.
         * @param expr0 Operand 0.
         * @param expr1 Operand 1.
         * @param expr2 Operand 2.
         * @return Created expression.
         */
        template<typename T0, typename T1, typename T2>
        LSExpression createExpression(LSOperator op, T0 expr0, T1 expr1, T2 expr2) {
            LSExpression expr(bsolver, ls_create_expression(bsolver, op));
            expr.addOperand(expr0);
            expr.addOperand(expr1);
            expr.addOperand(expr2);
            return expr;
        }
#endif

        /**
         * Creates a native function. The argument must be derived from
         * {@link LSNativeFunction}. When the native function is called,
         * the argument values will be made accessible to your function
         * through the native context.
         * 
         * Once you have instantiated it, you have to use {@link call} to call it in your model.
         *
         * @param func Native function to call.
         * @return The expression associated to the function.
         * @see LSNativeFunction
         * @see #O_NativeFunction
         * @since 6.0
         */
        LSExpression createNativeFunction(LSNativeFunction* func) {
            return LSExpression(bsolver, ls_create_native_function(bsolver, &LSModel::nativeFuncDispatch, (void*)func));
        }

#ifdef LS_CPP11
        /**
         * Creates a native function. The argument must be a std::function taking any
         * combination of lsint and lsdouble arguments. When the native function is called,
         * the argument values will be passed to your function as its arguments.
         * 
         * Once you have instantiated it, you have to use {@link call} to call it in your model.
         *
         * @param func std::function to call. It must only accept lsint and lsdouble arguments.
         * @return The expression associated to the function.
         * @see LSNativeFunction
         * @see #O_NativeFunction
         * @since 8.0
         */
        template <typename ...ARGS>
        LSExpression createNativeFunction(const std::function<lsdouble(ARGS...)> &func);
        
        /**
         * Creates a native function. The argument must be a std::function taking a
         * single native context as its argument. When the native function is called,
         * the argument values will be made accessible to your function through the
         * native context.
         * 
         * Once you have instantiated it, you have to use {@link call} to call it in your model.
         *
         * @param func Native function to call.
         * @return The expression associated to the function.
         * @see LSNativeFunction
         * @see #O_NativeFunction
         * @since 8.0
         */
        LSExpression createNativeFunction(const std::function<lsdouble(const LSNativeContext&)> &func);
        
        /**
         * Creates a function. A function is a particular expression composed of
         * two parts:
         * - The arguments of the function (which are also LSExpressions of type {@link #O_Argument})
         * - The body of the function. The body is an LSExpression that will be used to
         *   evaluate the result of the function. The body can be any LSExpression composed
         *   of any operands and operators supported by LocalSolver. Thus, the body
         *   expression can use the arguments of the function but can also capture and
         *   refer to expressions declared outside of the function.
         * 
         * You have to provide the body of the function as a std::function
         * (C++ function or lambda). Please note that the provided std::function
         * will not be used directly during the solving process, but will be evaluated
         * once by the API with a number of LSExpression of type {@link #O_Argument} that
         * corresponds to the number of arguments your std::function expects.
         * The returned LSExpression resulting of this evaluation will be used
         * as the body of the LocalSolver function {@link #O_Function}.
         *
         * @param functor Functor called to create the function.
         * @return The expression associated to the function.
         * @since 7.0
         */
        template<typename A>
        LSExpression createFunction(const A& functor) {
            return function_creator<decltype(&A::operator())>::create(this, functor);
        }

#endif

        /**
         * Creates a boolean decision.
         * Binary decision variable with domain { 0, 1 }.
         * This method is a shortcut for `createExpression(O_Bool)`.
         *
         * @see #O_Bool
         * @see #createExpression
         * @since 5.5
         */
        LSExpression boolVar() {
            return createExpression(O_Bool);
        }

        /**
         * Creates a float decision. Decision variable with domain [lb, ub].
         * This method is a shortcut for `createExpression(O_Float, lb, ub)`.
         *
         * @param lb Lower bound of the decision variable.
         * @param ub Upper bound of the decision variable.
         * @see #O_Float
         * @see #createExpression
         * @since 5.5
         */
        LSExpression floatVar(lsdouble lb, lsdouble ub) {
            return createExpression(O_Float, lb, ub);
        }

        /**
         * Creates an integer decision. Decision variable with domain [lb, ub].
         * This method is a shortcut for `createExpression(O_Int, lb, ub)`.
         *
         * @param lb Lower bound of the decision variable.
         * @param ub Upper bound of the decision variable.
         * @see #O_Int
         * @see #createExpression
         * @since 5.5
         */
        LSExpression intVar(lsint lb, lsint ub) {
            return createExpression(O_Int, lb, ub);
        }

#ifdef LS_CPP11

        /**
         * Creates a sum expression.
         * This method is a shortcut for `createExpression(O_Sum, operands)`.
         *
         * @see #O_Sum
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression sum(TN... operands) {
            return createExpression(O_Sum, operands...);
        }
#else

        /**
         * Creates a sum expression.
         * This method is a shortcut for `createExpression(O_Sum)`.
         *
         * @see #O_Sum
         * @see #createExpression
         * @since 5.5
         */
        LSExpression sum() {
            return createExpression(O_Sum);
        }

        /**
         * Creates a sum expression.
         * This method is a shortcut for `createExpression(O_Sum, expr0)`.
         *
         * @see #O_Sum
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression sum(T0 expr0) {
            return createExpression(O_Sum, expr0);
        }

        /**
         * Creates a sum expression.
         * This method is a shortcut for `createExpression(O_Sum, expr0, expr1)`.
         *
         * @see #O_Sum
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression sum(T0 expr0, T1 expr1) {
            return createExpression(O_Sum, expr0, expr1);
        }
#endif

        /**
         * Creates a substraction expression.
         * This method is a shortcut for `createExpression(O_Sub, expr0, expr1)`.
         *
         * @see #O_Sub
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression sub(T0 expr0, T1 expr1) {
            return createExpression(O_Sub, expr0, expr1);
        }

#ifdef LS_CPP11

        /**
         * Creates a call expression.
         * The first operand must be an LSExpression of type {@link #O_Function}
         * or {@link #O_NativeFunction}.
         * The other operands may be LSExpressions, booleans, integers, and doubles.
         * They are passed to the function as arguments.
         * 
         * This method is a shortcut for `createExpression(O_Call, operands)`.
         *
         * @see #O_Call
         * @see #createExpression
         * @since 6.0
         */
        template<typename... TN>
        LSExpression call(TN... operands) {
            return createExpression(O_Call, operands...);
        }

#else

        /**
         * Creates a call expression.
         * The first operand must be an LSExpression of type {@link #O_Function}
         * or {@link #O_NativeFunction}.
         * The other operands may be LSExpressions, booleans, integers, and doubles.
         * They are passed to the function as arguments.
         * 
         * This method is a shortcut for `createExpression(O_Call, expr0)`.
         *
         * @see #O_Call
         * @see #createExpression
         * @since 6.0
         */
        template<typename T0>
        LSExpression call(T0 expr0) {
            return createExpression(O_Call, expr0);
        }

        /**
         * Creates a call expression.
         * The first operand must be an LSExpression of type {@link #O_Function}
         * or {@link #O_NativeFunction}.
         * The other operands may be LSExpressions, booleans, integers, and doubles.
         * They are passed to the function as arguments.
         * 
         * This method is a shortcut for `createExpression(O_Call, expr0, expr1)`.
         *
         * @see #O_Call
         * @see #createExpression
         * @since 6.0
         */
        template<typename T0, typename T1>
        LSExpression call(T0 expr0, T1 expr1) {
            return createExpression(O_Call, expr0, expr1);
        }

        /**
         * Creates a call expression.
         * The first operand must be an LSExpression of type {@link #O_Function}
         * or {@link #O_NativeFunction}.
         * The other operands may be LSExpressions, booleans, integers, and doubles.
         * They are passed to the function as arguments.
         * 
         * This method is a shortcut for `createExpression(O_Call, expr0, expr1, expr2)`.
         *
         * @see #O_Call
         * @see #createExpression
         * @since 6.0
         */
        template<typename T0, typename T1, typename T2>
        LSExpression call(T0 expr0, T1 expr1, T2 expr2) {
            LSExpression expr = createExpression(O_Call, expr0);
            expr.addOperands(expr1, expr2);
            return expr;
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a product expression.
         * This method is a shortcut for `createExpression(O_Prod, operands)`.
         *
         * @see #O_Prod
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression prod(TN... operands) {
            return createExpression(O_Prod, operands...);
        }

#else

        /**
         * Creates a product expression.
         * This method is a shortcut for `createExpression(O_Prod)`.
         *
         * @see #O_Prod
         * @see #createExpression
         * @since 5.5
         */
        LSExpression prod() {
            return createExpression(O_Prod);
        }

        /**
         * Creates a product expression.
         * This method is a shortcut for `createExpression(O_Prod, expr0)`.
         *
         * @see #O_Prod
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression prod(T0 expr0) {
            return createExpression(O_Prod, expr0);
        }

        /**
         * Creates a product expression.
         * This method is a shortcut for `createExpression(O_Prod, expr0, expr1)`.
         *
         * @see #O_Prod
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression prod(T0 expr0, T1 expr1) {
            return createExpression(O_Prod, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a maximum expression.
         * This method is a shortcut for `createExpression(O_Max, operands)`.
         *
         * @see #O_Max
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression max(TN... operands) {
            return createExpression(O_Max, operands...);
        }

#else

        /**
         * Creates a maximum expression.
         * This method is a shortcut for `createExpression(O_Max)`.
         *
         * @see #O_Max
         * @see #createExpression
         * @since 5.5
         */
        LSExpression max() {
            return createExpression(O_Max);
        }

        /**
         * Creates a maximum expression.
         * This method is a shortcut for `createExpression(O_Max, expr0)`.
         *
         * @see #O_Max
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression max(T0 expr0) {
            return createExpression(O_Max, expr0);
        }

        /**
         * Creates a maximum expression.
         * This method is a shortcut for `createExpression(O_Max, expr0, expr1)`.
         *
         * @see #O_Max
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression max(T0 expr0, T1 expr1) {
            return createExpression(O_Max, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a minimum expression.
         * This method is a shortcut for `createExpression(O_Min, operands)`.
         *
         * @see #O_Min
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression min(TN... operands) {
            return createExpression(O_Min, operands...);
        }

#else

        /**
         * Creates a minimum expression.
         * This method is a shortcut for `createExpression(O_Min)`.
         *
         * @see #O_Min
         * @see #createExpression
         * @since 5.5
         */
        LSExpression min() {
            return createExpression(O_Min);
        }

        /**
         * Creates a minimum expression.
         * This method is a shortcut for `createExpression(O_Min, expr0)`.
         *
         * @see #O_Min
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression min(T0 expr0) {
            return createExpression(O_Min, expr0);
        }

        /**
         * Creates a minimum expression.
         * This method is a shortcut for `createExpression(O_Min, expr0, expr1)`.
         *
         * @see #O_Min
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression min(T0 expr0, T1 expr1) {
            return createExpression(O_Min, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a OR expression.
         * This method is a shortcut for `createExpression(O_Or, operands)`.
         *
         * @see #O_Or
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression or_(TN... operands) {
            return createExpression(O_Or, operands...);
        }

#else

        /**
         * Creates a OR expression.
         * This method is a shortcut for `createExpression(O_Or)`.
         *
         * @see #O_Or
         * @see #createExpression
         * @since 5.5
         */
        LSExpression or_() {
            return createExpression(O_Or);
        }

        /**
         * Creates a OR expression.
         * This method is a shortcut for `createExpression(O_Or, expr0)`.
         *
         * @see #O_Or
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression or_(T0 expr0) {
            return createExpression(O_Or, expr0);
        }

        /**
         * Creates a OR expression.
         * This method is a shortcut for `createExpression(O_Or, expr0, expr1)`.
         *
         * @see #O_Or
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression or_(T0 expr0, T1 expr1) {
            return createExpression(O_Or, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates an AND expression.
         * This method is a shortcut for `createExpression(O_And, operands)`.
         *
         * @see #O_And
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression and_(TN... operands) {
            return createExpression(O_And, operands...);
        }

#else

        /**
         * Creates an AND expression.
         * This method is a shortcut for `createExpression(O_And)`.
         *
         * @see #O_And
         * @see #createExpression
         * @since 5.5
         */
        LSExpression and_() {
            return createExpression(O_And);
        }

        /**
         * Creates an AND expression.
         * This method is a shortcut for `createExpression(O_And, expr0)`.
         *
         * @see #O_And
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression and_(T0 expr0) {
            return createExpression(O_And, expr0);
        }

        /**
         * Creates an AND expression.
         * This method is a shortcut for `createExpression(O_And, expr0, expr1)`.
         *
         * @see #O_And
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression and_(T0 expr0, T1 expr1) {
            return createExpression(O_And, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a XOR expression.
         * This method is a shortcut for `createExpression(O_Xor, operands)`.
         *
         * @see #O_Xor
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression xor_(TN... operands) {
            return createExpression(O_Xor, operands...);
        }

#else
        /**
         * Creates a XOR expression.
         * This method is a shortcut for `createExpression(O_Xor)`.
         *
         * @see #O_Xor
         * @see #createExpression
         * @since 5.5
         */
        LSExpression xor_() {
            return createExpression(O_Xor);
        }
        /**
         * Creates a XOR expression.
         * This method is a shortcut for `createExpression(O_Xor, expr0)`.
         *
         * @see #O_Xor
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression xor_(T0 expr0) {
            return createExpression(O_Xor, expr0);
        }

        /**
         * Creates a XOR expression.
         * This method is a shortcut for `createExpression(O_Xor, expr0, expr1)`.
         *
         * @see #O_Xor
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression xor_(T0 expr0, T1 expr1) {
            return createExpression(O_Xor, expr0, expr1);
        }

#endif

        /**
         * Creates a NOT expression.
         * This method is a shortcut for `createExpression(O_Not, expr0)`.
         *
         * @see #O_Not
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression not_(T0 expr0) {
            return createExpression(O_Not, expr0);
        }

        /**
         * Creates an equality expression.
         * This method is a shortcut for `createExpression(O_Eq, expr0, expr1)`.
         *
         * @see #O_Eq
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression eq(T0 expr0, T1 expr1) {
            return createExpression(O_Eq, expr0, expr1);
        }

        /**
         * Creates a disequality expression.
         * This method is a shortcut for `createExpression(O_Neq, expr0, expr1)`.
         *
         * @see #O_Neq
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression neq(T0 expr0, T1 expr1) {
            return createExpression(O_Neq, expr0, expr1);
        }

        /**
         * Creates an inequality expression greater than or equal to.
         * This method is a shortcut for `createExpression(O_Geq, expr0, expr1)`.
         *
         * @see #O_Geq
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression geq(T0 expr0, T1 expr1) {
            return createExpression(O_Geq, expr0, expr1);
        }

        /**
         * Creates an inequality expression less than or equal to.
         * This method is a shortcut for `createExpression(O_Leq, expr0, expr1)`.
         *
         * @see #O_Leq
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression leq(T0 expr0, T1 expr1) {
            return createExpression(O_Leq, expr0, expr1);
        }

        /**
         * Creates an inequality expression greater than.
         * This method is a shortcut for `createExpression(O_Gt, expr0, expr1)`.
         *
         * @see #O_Gt
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression gt(T0 expr0, T1 expr1) {
            return createExpression(O_Gt, expr0, expr1);
        }

        /**
         * Creates an inequality expression less than.
         * This method is a shortcut for `createExpression(O_Lt, expr0, expr1)`.
         *
         * @see #O_Lt
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression lt(T0 expr0, T1 expr1) {
            return createExpression(O_Lt, expr0, expr1);
        }

        /**
         * Creates a ternary conditional expression.
         * This method is a shortcut for `createExpression(O_If, condExpr, trueExpr, falseExpr)`.
         *
         * @see #O_If
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1, typename T2>
        LSExpression iif(T0 condExpr, T1 trueExpr, T2 falseExpr) {
            return createExpression(O_If, condExpr, trueExpr, falseExpr);
        }

        /**
         * Creates an absolute value expression.
         * This method is a shortcut for `createExpression(O_Abs, expr0)`.
         *
         * @see #O_Abs
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression abs(T0 expr0) {
            return createExpression(O_Abs, expr0);
        }

        /**
         * Creates a distance expression.
         * This method is a shortcut for `createExpression(O_Dist, expr0, expr1)`.
         *
         * @see #O_Dist
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression dist(T0 expr0, T1 expr1) {
            return createExpression(O_Dist, expr0, expr1);
        }

        /**
         * Creates a division expression.
         * This method is a shortcut for `createExpression(O_Div, expr0, expr1)`.
         *
         * @see #O_Div
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression div(T0 expr0, T1 expr1) {
            return createExpression(O_Div, expr0, expr1);
        }

        /**
         * Creates a modulo expression.
         * This method is a shortcut for `createExpression(O_Mod, expr0, expr1)`.
         *
         * @see #O_Mod
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression mod(T0 expr0, T1 expr1) {
            return createExpression(O_Mod, expr0, expr1);
        }


#ifdef LS_CPP11

        /**
         * Creates an array expression.
         * This method is a shortcut for `createExpression(O_Array, operands)`.
         *
         * @see #O_Array
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression array(TN... operands) {
            return createExpression(O_Array, operands...);
        }

#else

        /**
         * Creates an array expression.
         * This method is a shortcut for `createExpression(O_Array)`.
         *
         * @see #O_Array
         * @see #createExpression
         * @since 5.5
         */
        LSExpression array() {
            return createExpression(O_Array);
        }

        /**
         * Creates an array expression.
         * This method is a shortcut for `createExpression(O_Array, expr0)`.
         *
         * @see #O_Array
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression array(T0 expr0) {
            return createExpression(O_Array, expr0);
        }

        /**
         * Creates an array expression.
         * This method is a shortcut for `createExpression(O_Array, expr0, expr1)`.
         *
         * @see #O_Array
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression array(T0 expr0, T1 expr1) {
            return createExpression(O_Array, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a "at" expression for N-dimensional array.
         * This method is a shortcut for `createExpression(O_At, arrayExpr, operands)`.
         *
         * @see #O_At
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename... TN>
        LSExpression at(T0 arrayExpr, TN... operands) {
            return createExpression(O_At, arrayExpr, operands...);
        }

#else

        /**
         * Creates a "at" expression for 1-dimensional array.
         * This method is a shortcut for `createExpression(O_At, arrayExpr, index1)`.
         *
         * @see #O_At
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression at(T0 arrayExpr, T1 index1) {
            return createExpression(O_At, arrayExpr, index1);
        }

        /**
         * Creates a "at" expression for 2-dimensional array.
         * This method is a shortcut for `createExpression(O_At, arrayExpr, index1, index2)`.
         *
         * @see #O_At
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1, typename T2>
        LSExpression at(T0 arrayExpr, T1 index1, T2 index2) {
            return createExpression(O_At, arrayExpr, index1, index2);
        }

        /**
         * Creates a "at" expression for 3-dimensional array.
         * This method is a shortcut for `createExpression(O_At, arrayExpr, index1, index2, index3)`.
         *
         * @see #O_At
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1, typename T2, typename T3>
        LSExpression at(T0 arrayExpr, T1 index1, T2 index2, T3 index3) {
            LSExpression expr(bsolver, ls_create_expression(bsolver, O_At));
            expr.addOperand(arrayExpr);
            expr.addOperand(index1);
            expr.addOperand(index2);
            expr.addOperand(index3);
            return expr;
        }

#endif

        /**
         * Creates an expression for the scalar product between two arrays.
         * This method is a shortcut for `createExpression(O_Scalar, expr0, expr1)`.
         *
         * @see #O_Scalar
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression scalar(T0 expr0, T1 expr1) {
            return createExpression(O_Scalar, expr0, expr1);
        }

        /**
         * Creates a ceil expression.
         * This method is a shortcut for `createExpression(O_Ceil, expr0)`.
         *
         * @see #O_Ceil
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression ceil(T0 expr0) {
            return createExpression(O_Ceil, expr0);
        }

        /**
         * Creates a floor expression.
         * This method is a shortcut for `createExpression(O_Floor, expr0)`.
         *
         * @see #O_Floor
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression floor(T0 expr0) {
            return createExpression(O_Floor, expr0);
        }

        /**
         * Creates a rounding expression.
         * This method is a shortcut for `createExpression(O_Round, expr0)`.
         *
         * @see #O_Round
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression round(T0 expr0) {
            return createExpression(O_Round, expr0);
        }

        /**
         * Creates a square root expression.
         * This method is a shortcut for `createExpression(O_Sqrt, expr0)`.
         *
         * @see #O_Sqrt
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression sqrt(T0 expr0) {
            return createExpression(O_Sqrt, expr0);
        }

        /**
         * Creates a log expression.
         * This method is a shortcut for `createExpression(O_Log, expr0)`.
         *
         * @see #O_Log
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression log(T0 expr0) {
            return createExpression(O_Log, expr0);
        }

        /**
         * Creates an exponential expression.
         * This method is a shortcut for `createExpression(O_Exp, expr0)`.
         *
         * @see #O_Exp
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression exp(T0 expr0) {
            return createExpression(O_Exp, expr0);
        }

        /**
         * Creates a power expression.
         * This method is a shortcut for `createExpression(O_Pow, expr0, expr1)`.
         *
         * @see #O_Pow
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression pow(T0 expr0, T1 expr1) {
            return createExpression(O_Pow, expr0, expr1);
        }

        /**
         * Creates a cosine expression.
         * This method is a shortcut for `createExpression(O_Cos, expr0)`.
         *
         * @see #O_Cos
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression cos(T0 expr0) {
            return createExpression(O_Cos, expr0);
        }

        /**
         * Creates a sine expression.
         * This method is a shortcut for `createExpression(O_Sin, expr0)`.
         *
         * @see #O_Sin
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression sin(T0 expr0) {
            return createExpression(O_Sin, expr0);
        }

        /**
         * Creates a tangent expression.
         * This method is a shortcut for `createExpression(O_Tan, expr0)`.
         *
         * @see #O_Tan
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression tan(T0 expr0) {
            return createExpression(O_Tan, expr0);
        }

        /**
         * Creates a piecewise linear expression.
         * This method is a shortcut for `createExpression(O_Piecewise, expr, expr1, expr2)`.
         *
         * @see #O_Piecewise
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1, typename T2>
        LSExpression piecewise(T0 expr0, T1 expr1, T2 expr2) {
            return createExpression(O_Piecewise, expr0, expr1, expr2);
        }

        /**
         * Creates a list decision with the given length.
         * A list is a ordered collection of integers within a domain [0, n-1].
         * This method is a shortcut for `createExpression(O_List, n)`.
         *
         * @see #O_List
         * @see #createExpression
         * @since 5.5
         */
        LSExpression listVar(lsint n) {
            return createExpression(O_List, n);
        }
        
        /**
         * Creates a set decision with the given length.
         * A set is a unordered collection of integers within a domain [0, n-1].
         * This method is a shortcut for `createExpression(O_Set, n)`.
         *
         * @see #O_Set
         * @see #createExpression
         * @since 8.0
         */
        LSExpression setVar(lsint n) {
            return createExpression(O_Set, n);
        }

        /**
         * Creates a count expression.
         * This method is a shortcut for `createExpression(O_Count, expr0)`.
         *
         * @see #O_Count
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0>
        LSExpression count(T0 expr0) {
            return createExpression(O_Count, expr0);
        }

        /**
         * Creates an indexOf expression.
         * This method is a shortcut for `createExpression(O_IndexOf, expr0, expr1)`.
         *
         * @see #O_IndexOf
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression indexOf(T0 expr0, T1 expr1) {
            return createExpression(O_IndexOf, expr0, expr1);
        }
        
        /**
         * Creates a contains expression.
         * This method is a shortcut for `createExpression(O_Contains, expr0, expr1)`.
         *
         * @see #O_Contains
         * @see #createExpression
         * @since 7.5
         */
        template<typename T0, typename T1>
        LSExpression contains(T0 expr0, T1 expr1) {
            return createExpression(O_Contains, expr0, expr1);
        }

#ifdef LS_CPP11

        /**
         * Creates a partition expression.
         * This method is a shortcut for `createExpression(O_Partition, operands)`.
         *
         * @see #O_Partition
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression partition(TN... operands) {
            return createExpression(O_Partition, operands...);
        }

#else

        /**
         * Creates a partition expression.
         * This method is a shortcut for `createExpression(O_Partition)`.
         *
         * @see #O_Partition
         * @see #createExpression
         * @since 5.5
         */
        LSExpression partition() {
            return createExpression(O_Partition);
        }

        /**
         * Creates a partition expression.
         * This method is a shortcut for `createExpression(O_Partition, expr0, expr1)`.
         *
         * @see #O_Partition
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression partition(T0 expr0, T1 expr1) {
            return createExpression(O_Partition, expr0, expr1);
        }

#endif

#ifdef LS_CPP11

        /**
         * Creates a disjoint expression.
         * This method is a shortcut for `createExpression(O_Disjoint, operands)`.
         *
         * @see #O_Disjoint
         * @see #createExpression
         * @since 5.5
         */
        template<typename... TN>
        LSExpression disjoint(TN... operands) {
            return createExpression(O_Disjoint, operands...);
        }

#else

        /**
         * Creates a disjoint expression.
         * This method is a shortcut for `createExpression(O_Disjoint)`.
         *
         * @see #O_Disjoint
         * @see #createExpression
         * @since 5.5
         */
        LSExpression disjoint() {
            return createExpression(O_Disjoint);
        }


        /**
         * Creates a disjoint expression.
         * This method is a shortcut for `createExpression(O_Disjoint, expr0, expr1)`.
         *
         * @see #O_Disjoint
         * @see #createExpression
         * @since 5.5
         */
        template<typename T0, typename T1>
        LSExpression disjoint(T0 expr0, T1 expr1) {
            return createExpression(O_Disjoint, expr0, expr1);
        }

#endif

        /**
         * Creates a native function expression.
         * This method is a shortcut for `createNativeFunction(func)`.
         *
         * @see #O_NativeFunction
         * @see #createNativeFunction
         * @since 6.0
         */
        LSExpression nativeFunction(LSNativeFunction* func) {
            return createNativeFunction(func);
        }

#ifdef LS_CPP11
        /**
         * Creates a native function expression.
         * This method is a shortcut for `createNativeFunction(func)`.
         *
         * @see #O_NativeFunction
         * @see #createNativeFunction
         * @since 8.0
         */
        template <typename ...ARGS>
        LSExpression nativeFunction(const std::function<lsdouble(ARGS...)> &func) {
            return createNativeFunction(func);
        }
        
        /**
         * Creates a native function expression.
         * This method is a shortcut for `createNativeFunction(func)`.
         *
         * @see #O_NativeFunction
         * @see #createNativeFunction
         * @since 8.0
         */
        LSExpression nativeFunction(const std::function<lsdouble(const LSNativeContext&)> &func) {
            return createNativeFunction(func);
        }
        
        /**
         * Creates a function expression.
         * This method is a shortcut for `createFunction(functor)`.
         *
         * @see #O_Function
         * @see #createFunction
         * @since 7.0
         */
        template<typename T>
        LSExpression function(T functor) {
            return createFunction(functor);
        }

#endif

        /**
         * Creates a range expression, where expr0 is the lower bound (inclusive) and
         * expr1 is the upper bound (exclusive).
         * This method is a shortcut for `createExpression(O_Range, expr0, expr1)`.
         *
         * @see #O_Range
         * @see #createExpression
         * @since 7.0
         */
        template<typename T0, typename T1>
        LSExpression range(T0 expr0, T1 expr1) {
            return createExpression(O_Range, expr0, expr1);
        }

        /**
         * Gets the number of expressions added to this model.
         *
         * @return Number of expressions.
         */
        int getNbExpressions() const {
            return ls_nb_expressions(bsolver);
        }

        /**
         * Gets the expression with the given index in this model.
         *
         * @param exprIndex Index of the expression.
         * @return Expression with the given index.
         */
        LSExpression getExpression(int exprIndex) const {
            return LSExpression(bsolver, exprIndex);
        }

        /**
         * Gets the expression with the given name.
         * Throws an exception if no expression with the given name exists.
         *
         * @param name Name.
         * @return Expression with the given name.
         */
        LSExpression getExpression(const std::string& name) const {
            return LSExpression(bsolver, ls_get_expression_with_name(bsolver, name.c_str()));
        }

        /**
         * Gets the number of decisions in the model. This corresponds to
         * the number of decision variables declared in the model.
         *
         * @return Number of decisions.
         */
        int getNbDecisions() const {
            return ls_nb_decisions(bsolver);
        }

        /**
         * Gets the decision with the given index.
         *
         * @param decisionIndex Index of the decision.
         * @return Decision with the given index.
         */
        LSExpression getDecision(int decisionIndex) const {
            return LSExpression(bsolver, ls_get_decision(bsolver, decisionIndex));
        }

        /**
         * Adds the given expression to the list of constraints. It means that
         * the value of this expression must be constrained to be equal to 1 in
         * any solution found by the solver. Hence, only boolean expressions
         * (that is, expressions whose value is boolean) can be constrained.
         * Only allowed in state {@link #S_Modeling}. If the expression is already a constraint,
         * this method does nothing and returns immediately.
         *
         * Try to avoid hard constraints as much as possible, because %LocalSolver
         * (and more generally local search) is not suited for solving hardly
         * constrained problems. In particular, banish constraints that are not
         * surely satisfied in practice. Ideally, only combinatorial constraints
         * (which induce the combinatorial structure of your problem) have to be set.
         * All the other constraints can be relaxed as primary objectives
         * in order to be "softly" satisfied (goal programming). For instance,
         * constraint a &lt;= b can be transformed into minimize max(b-a, 0).
         *
         *  @param expr Expression.
         */
        void addConstraint(const LSExpression& expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_add_constraint(bsolver, expr.exprId);
        }

        /**
         * Shortcut for `addConstraint(expr)`.
         *
         * @see #addConstraint
         * @param expr Expression.
         * @since 5.5
         */
        void constraint(const LSExpression& expr) {
            addConstraint(expr);
        }

        /**
         * Removes the given expression from the list of constraints. If the expression
         * was not constrained, this method does nothing and returns immediately.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @since 5.0
         * @param expr Expression.
         */
        void removeConstraint(const LSExpression& expr) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_remove_constraint_with_expr(bsolver, expr.exprId);
        }

        /**
         * Removes the constraint at the given position in the list of constraints.
         * Only allowed in state {@link #S_Modeling}.
         *
         * @since 5.0
         * @param constraintIndex position of the constraint to remove.
         */
        void removeConstraint(int constraintIndex) {
            ls_remove_constraint(bsolver, constraintIndex);
        }

        /**
         * Gets the number of constraints added to this model.
         *
         * @return Number of constraints.
         */
        int getNbConstraints() const {
            return ls_nb_constraints(bsolver);
        }

        /**
         * Gets the constraint with the given index.
         *
         * @param constraintIndex Index of the constraint.
         * @return Constraint with the given index.
         */
        LSExpression getConstraint(int constraintIndex) const {
            return LSExpression(bsolver, ls_get_constraint(bsolver, constraintIndex));
        }

        /**
         * Adds the given expression to the list of objectives to optimize. The same expression
         * can be added more than once. Only allowed in state {@link #S_Modeling}. Note that the objectives
         * will be optimized in the order in which they have been added to the model. It is useful
         * for lexicographic multiobjective optimization, and more particularly for goal programming.
         *
         * @param expr Expression.
         * @param direction Optimization direction of this objective.
         */
        void addObjective(const LSExpression& expr, LSObjectiveDirection direction) {
            LS_CHECK_SAME_MODEL(bsolver, expr.bsolver);
            ls_add_objective(bsolver, expr.exprId, direction);
        }

        /**
         * Shortcut for `addObjective(expr, OD_Minimize)`.
         *
         * @see #addObjective
         * @param expr Expression.
         * @since 5.5
         */
        void minimize(const LSExpression& expr) {
            addObjective(expr, OD_Minimize);
        }

        /**
         * Shortcut for `addObjective(expr, OD_Maximize)`.
         *
         * @see #addObjective
         * @param expr Expression.
         * @since 5.5
         */
        void maximize(const LSExpression& expr) {
            addObjective(expr, OD_Maximize);
        }

        /**
         * Removes the objective at the given position in the list of objectives.
         * Note that the objectives created after the removed one have their index decreased
         * by 1.
         * Phases are not modified when an objective is removed. It is the
         * user's responsibility to change the objective index of each phase to keep it coherent
         * (with {@link LSPhase#setOptimizedObjective}), or to disable it (with
         * {@link LSPhase#setEnabled}).
         * Only allowed in state {@link #S_Modeling}.
         *
         * @since 5.0
         * @param objectiveIndex position of the objective to remove.
         */
        void removeObjective(int objectiveIndex) const {
            ls_remove_objective(bsolver, objectiveIndex);
        }

        /**
         * Gets the number of objectives added to this model.
         *
         * @return Number of objectives.
         */
        int getNbObjectives() const {
            return ls_nb_objectives(bsolver);
        }

        /**
         * Gets the objective with the given index.
         *
         * @param objectiveIndex Index of the objective.
         * @return Objective with the given index.
         */
        LSExpression getObjective(int objectiveIndex) const {
            return LSExpression(bsolver, ls_get_objective(bsolver, objectiveIndex));
        }

        /**
         * Gets the direction of the objective with the given index.
         *
         * @param objectiveIndex Index of the objective.
         * @return Objective direction.
         */
        LSObjectiveDirection getObjectiveDirection(int objectiveIndex) const {
            return ls_get_objective_direction(bsolver, objectiveIndex);
        }

        /**
         * Gets the number of operands in the model. This corresponds to the number of operands
         * for all expressions declared in the model. It is an analog of the number of non zeros
         * in matrix model encountered in mathematical programming: it gives an hint about the
         * size and the density of your model.
         *
         * @return Number of operands.
         */
        int getNbOperands() const {
            return ls_nb_operands(bsolver);
        }

        /**
         * Closes the model. Only allowed in state {@link #S_Modeling}. When this
         * method is called, the solver is placed in state {@link #S_Stopped}.
         * 
         * The solution is reset and any initial solution is lost. Once the model is closed,
         * no expression, constraints or objectives can be added or removed unless the model
         * is reopened.  The model must be closed before starting its resolution.
         */
        void close() {
            ls_close(bsolver);
        }

        /**
         * Reopens the model. Only allowed in state {@link #S_Stopped}. When this
         * method is called, the solver is placed in state {@link #S_Modeling}.
         * 
         * In this state, the model can be modified: it is possible to add new expressions,
         * constraints or objectives, modify expression operands, and remove existing
         * constraints and objectives. However, existing expressions cannot be deleted.
         * Note that the solution will be reset when the model is closed again.
         */
        void open() {
            ls_open(bsolver);
        }

        /**
         * Returns true if the model is closed, false otherwise.
         *
         * @return True if the model is closed.
         */
        bool isClosed() const {
            return ls_is_closed(bsolver);
        }

        /**
         * Returns a string representation of this model. This representation provides:
         * - The number of expressions, decisions, constraints, and objectives.
         * - The density of the model.
         * Useful for debugging or logging purposes.
         *
         * @return String representation.
         */
        std::string toString() const {
            int size = ls_model_to_string(bsolver, NULL, 0);
            std::string str(size, '\0');
            ls_model_to_string(bsolver, &str[0], size);
            return str;
        }

        ~LSModel() { }
        LSModel() { bsolver = NULL; disposables = NULL; }
        LSModel(const LSModel& other) { bsolver = other.bsolver; disposables = other.disposables; }
        LSModel& operator=(const LSModel& other) { bsolver = other.bsolver; disposables = other.disposables; return *this; }

    private:
        friend class LocalSolver;
        
        struct DisposableHolder {
            virtual ~DisposableHolder() {}
        };

        explicit LSModel(lssolver* solver, std::vector<DisposableHolder*> *disposables) : bsolver(solver), disposables(disposables) {
            ls_check_not_running(bsolver);
        }

        lssolver* bsolver;
        std::vector<DisposableHolder*> *disposables;

        static lsdouble nativeFuncDispatch(lssolver* solver, lsbuffer* buffer, void* userdata) {
            LSNativeContext context(solver, buffer);
            return ((LSNativeFunction*)userdata)->call(context);
        }

#ifdef LS_CPP11

        template<std::size_t ...>
        struct sequence { };

        template<std::size_t N, std::size_t ...S>
        struct generator : generator<N-1, N-1, S...> { };

        template<std::size_t ...S>
        struct generator<0, S...> {
            typedef sequence<S...> type;
        };
        

        template<typename T>
        struct function_creator;

        template<typename R, typename C, typename ...ARGS>
        struct function_creator<R(C::*)(ARGS...) const> {
            static LSExpression create(LSModel* model, const C& functor) {
                LSExpression expr = model->createExpression(O_Function);
                std::array<LSExpression, sizeof...(ARGS)> args { create_argument<ARGS>(model, expr)... };
                LSExpression ret = call_function(functor, args, typename generator<sizeof...(ARGS)>::type());
                expr.addOperand(ret);
                return expr;
            }
        };

        template<typename T>
        static LSExpression create_argument(LSModel* model, LSExpression func) {
            LSExpression arg = model->createExpression(O_Argument);
            func.addOperand(arg);
            return arg;
        }

        template<typename C, std::size_t N, std::size_t... IDX>
        static LSExpression call_function(const C& functor, const std::array<LSExpression, N>& args, sequence<IDX...>) {
            return functor(std::get<IDX>(args)...);
        }
        
        template <typename T>
        static T extractArg(const LSNativeContext &context, std::size_t ind);
        
        template <typename... ARGS, std::size_t... IDX>
        static lsdouble variadicFuncDispatchImpl(const std::function<lsdouble(ARGS...)> &func, const LSNativeContext &context, sequence<IDX...>) {
            return func(extractArg<ARGS>(context, IDX)...);
        }
        
        template <typename ...ARGS>
        static lsdouble variadicFuncDispatch(lssolver* solver, lsbuffer* buffer, void* userdata) {
            typedef std::function<lsdouble(ARGS...)> Func;
            Func* func = (Func*) userdata;
            LSNativeContext context(solver, buffer);
            return variadicFuncDispatchImpl(*func, context, typename generator<sizeof...(ARGS)>::type());
        }
        
        static lsdouble lambdaFuncDispatch(lssolver* solver, lsbuffer* buffer, void* userdata) {
            typedef std::function<lsdouble(const LSNativeContext&)> Func;
            LSNativeContext context(solver, buffer);
            return ((Func*)userdata)->operator()(context);
        }
        
        template <typename ...ARGS>
        struct NativeFunctionHolder : DisposableHolder {
            NativeFunctionHolder(const std::function<lsdouble(ARGS...)> &func) { function = func; }
            std::function<lsdouble(ARGS...)> function;
        };
#endif
    };

#ifdef LS_CPP11
    template <>
    inline lsdouble LSModel::extractArg<lsdouble>(const LSNativeContext& context, std::size_t ind) {
        return context.isDouble(ind) ? context.getDoubleValue(ind) : (lsdouble)context.getIntValue(ind);
    }
    template <>
    inline lsint LSModel::extractArg<lsint>(const LSNativeContext& context, std::size_t ind) {
    return context.getIntValue(ind);
    }
    
    template <typename ...ARGS>
    inline LSExpression LSModel::createNativeFunction(const std::function<lsdouble(ARGS...)> &func) {
        NativeFunctionHolder<ARGS...> *holder = new NativeFunctionHolder<ARGS...>(func);
        disposables->push_back(holder);
        return LSExpression(bsolver, ls_create_native_function(bsolver, &LSModel::variadicFuncDispatch<ARGS...>, (void*) &(holder->function)));
    }
    
    inline LSExpression LSModel::createNativeFunction(const std::function<lsdouble(const LSNativeContext&)> &func) {
        NativeFunctionHolder<LSNativeContext> *holder = new NativeFunctionHolder<LSNativeContext>(func);
        disposables->push_back(holder);
        return LSExpression(bsolver, ls_create_native_function(bsolver, &LSModel::lambdaFuncDispatch, (void*) &(holder->function)));
    }
#endif

}

#endif
