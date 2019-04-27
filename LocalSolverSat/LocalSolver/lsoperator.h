// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSOPERATOR_H
#define LS_LSOPERATOR_H

namespace localsolver {

    /**
     * Mathematical operators available for modeling. These operators are used to type
     * the expressions created in a %LocalSolver mathematical optimization model.
     *
     * @see LSModel
     * @see LSExpression
     */
    enum LSOperator {
        /**
         * Boolean decision. Decisional operator with no operand.
         * Decision variable with domain `{0,1}`.
         */
        O_Bool,

        /**
         * Float decision. Operator with two operands that represent the lower bound
         * and the upper bound of the decision (domain `[lb, ub]`).
         * The bounds must be constants (integers or doubles).
         *
         * @since 4.0
         */
        O_Float,

        /**
         * Constant. Operator with no argument. Constants can be booleans, integers
         * or doubles. Note that constants 0 or 1 are considered as boolean.
         * Constants are implicitly created when passing lsint or lsdouble arguments
         * to {@link LSModel#createExpression} or {@link LSExpression#addOperand}.
         * They can also be created with the dedicated function {@link LSModel#createConstant}.
         */
        O_Const,

        /**
         * Sum. N-ary arithmetic operator. `SUM(e1, e2, ..., eN)` is equal to the sum
         * of all operands `e1, e2, ..., eN`. This operator returns an integer if all
         * the operands are booleans or integers and a double as soon as one
         * operand is a double.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will sum all the
         * values computed and returned by the function.
         */
        O_Sum,

        /**
         * Substraction. Binary arithmetic operator.
         * `SUB(x, y)` is equal to the value of `x - y`.
         * This operator returns an integer if the two operands are booleans or
         * integers, and a double as soon as one operand is a double.
         *
         * @since 4.0
         */
        O_Sub,

        /**
         * Product. N-ary arithmetic operator. `PROD(e1, e2, ..., eN)` is equal to
         * the product of all operands `e1, e2, ..., eN`. This operator returns an
         * integer if all the operands are booleans or integers, and a double as
         * soon as one operand is a double.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will compute the
         * product of all the values returned by the function.
         */
        O_Prod,

        /**
         * Maximum. N-ary arithmetic operator. `MAX(e1, e2, ..., eN)` is equal to
         * the maximum value among all operands `e1, e2, ..., eN`. This operator
         * returns an integer if all the operands are booleans or integers,
         * and a double as soon as one operand is a double.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will find the maximum
         * value among all the values returned by the function.
         */
        O_Max,

        /**
         * Minimum. N-ary arithmetic operator. `MIN(e1, e2, ..., eN)` is equal to
         * the minimum value among all operands `e1, e2, ..., eN`. This operator
         * returns an integer if all the operands are booleans or integers,
         * and a double as soon as one operand is a double.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will find the minimum
         * value among all the values returned by the function.
         */
        O_Min,

        /**
         * Equal. Binary relational operator.
         * `EQ(a,b) = 1` if `a == b`, and `0` otherwise.
         * This operator returns a boolean.
         */
        O_Eq,

        /**
         * Not equal to. Binary relational operator.
         * `NEQ(a,b) = 1` if `a != b`, and `0` otherwise.
         * This operator returns a boolean.
         */
        O_Neq,

        /**
         * Greater than or equal to. Binary relational operator.
         * `GEQ(a,b) = 1` if `a &gt;= b`, and `0` otherwise.
         * This operator returns a boolean.
         */
        O_Geq,

        /**
         * Lower than or equal to. Binary relational operator.
         * `LEQ(a,b) = 1` if `a &lt;= b`, and `0` otherwise.
         * This operator returns a boolean.
         */
        O_Leq,

        /**
         * Strictly greater than. Binary relational operator.
         * `GT(a,b) = 1` if `a &gt; b`, and `0` otherwise.
         * This operator returns a boolean.
         */
        O_Gt,

        /**
         * Strictly lower than. Binary relational operator.
         * `LQ(a, b) = 1` if `a &lt; b`, and `0` otherwise.
         * This operator returns a boolean.
         */
        O_Lt,

        /**
         * If-Then-Else. Ternary conditional operator.
         * `IF(a, b, c)` is equal to `b` if `a = 1`, and `c` otherwise. The first
         * operand must be a boolean (that is, equal to 0 or 1).
         * This operator returns a boolean if the three operands are booleans,
         * an integer if the second and third operands are integers, and a double
         * if the second or the third operand is a double.
         */
        O_If,

        /**
         * Not. Unary logical operator. `NOT(a) = 1 - a`.
         * The operand must be boolean (that is, equal to 0 or 1).
         * This operator returns a boolean.
         */
        O_Not,

        /**
         * And. N-ary logical operator. `AND(e1, e2, ..., eN)` is equal to 1 (true)
         * if all the operands `e1, e2, ..., eN` are 1, and 0 otherwise.
         * All the operands must be boolean (that is, equal to 0 or 1).
         * This operator returns a boolean.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will return 1 if all
         * the values returned by the function are 1 and 0 otherwise.
         */
        O_And,

        /**
         * Or. N-ary logical operator. `OR(e1, e2, ..., eN)` is equal to 0 (false)
         * if all operands `e1, e2, ..., eN` are 0, and 1 otherwise.
         * All the operands must be boolean (that is, equal to 0 or 1).
         * This operator returns a boolean.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will return 0 if
         * all the values returned by the function are 0 and 1 otherwise.
         */
        O_Or,

        /**
         * Exclusive or (also called "xor"). N-ary logical operator.
         * `XOR(e1, e2, ..., eN)` is equal to 0 if the number of operands with
         * value 1 among `e1, e2, ..., eN` is even, and 1 otherwise.
         * Remarkable case: `XOR(a,b) = 1` if `a == b`, and `0` otherwise.
         * All the operands must be boolean (that is, equal to 0 or 1).
         * This operator returns a boolean.
         *
         * ### With collections or ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create expressions with a dynamic number of operands.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and will return 0 if the
         * number of value 1 returned by the function is even, and 1 otherwise.
         */
        O_Xor,

        /**
         * Absolute value. Unary arithmetic operator. `ABS(e) = e &gt;= 0 ? e : -e`.
         * This operator returns an integer if the operand is a boolean or an integer,
         * and a double otherwise.
         */
        O_Abs,

        /**
         * Distance between two numbers. Binary arithmetic operator. `DIST(a,b) = ABS(a-b)`.
         * This operator returns an integer if the two operands are booleans or integers,
         * and a double as soon as one of the operand is a double.
         */
        O_Dist,

        /**
         * Division. Binary arithmetic operator.
         * This operator always returns a double. Note that until version 4.0,
         * the division was an integer division if both operands were integers.
         */
        O_Div,

        /**
         * Modulo (remainder of the integer division). Binary arithmetic operator.
         * `MOD(a,b) = r` such that `a = q * b + r` with `q`, `r` integers and `|r| &lt; b`.
         * The operands must be integers or booleans. This operator returns an integer.
         */
        O_Mod,

        /**
         * Array. An array is a collection of elements. Indexes begin at 0.
         * It could be used with operators like {@link #O_At} or {@link #O_Scalar}.
         * An array doesn't have a value by itself, but can contain operands
         * of type boolean, integer, double or array (for multi-dimensional arrays).
         * All the elements of an array must be of the same type.
         *
         * ### With ranges ###
         * This operator can also be used with {@link #O_Range}, {@link #O_List}
         * or {@link #O_Set} to create an array with a dynamic number of elements.
         * In that case, this operator becomes a binary operator that takes a range,
         * a list or a set as first operand and a function ({@link #O_Function} or
         * {@link #O_NativeFunction}) as second operand. The operator will call the
         * function on each value of the range, list or set and the returned values
         * will be used to populate the array.
         *
         * @since 2.1
         */
        O_Array,

        /**
         * Returns the element at specific coordinates of an array or a list.
         *
         * ### For arrays ###
         * The first operand must be the array and the other operands must be the
         * coordinates of the element to get. The number of coordinates depends
         * on the dimension of the array. Thus AT(myArray, i) returns the i element
         * of the one-dimensional array myArray. This operator returns a boolean,
         * an integer or a double according to the type of the operands in the array.
         * If one of the specified coordinate is out of range, the evaluation
         * of the expression will fail.
         *
         * ### For lists ###
         * The first operand must be the list and the second operand must be the
         * index of the element to get. If the index is out of range
         * (index < 0 or index > count(list)), the evaluation of the expression
         * will not fail but will return -1.
         *
         * @since 2.1
         */
        O_At,

        /**
         * Scalar product. `SCALAR(a, x) = sum(a[i]*x[i])` where `a` and `x` are two
         * one-dimensional arrays. This operator returns an integer or a double
         * according to the type of the operands in the arrays.
         *
         * @since 2.1
         */
        O_Scalar,

        /**
         * Ceil. Unary arithmetic operator. Returns a value rounded to the next
         * highest integer. The operand can be a boolean, an integer or a double.
         * This operator returns an integer.
         *
         * @since 3.0
         */
        O_Ceil,

        /**
         * Floor. Unary arithmetic operator. Returns a value rounded to the next
         * lowest integer. The operand can be a boolean, an integer or a double.
         * This operator returns an integer.
         *
         * @since 3.0
         */
        O_Floor,

        /**
         * Round. Unary arithmetic operator. Returns a value rounded to the
         * nearest integer. The operand can be a boolean, an integer or a double.
         * This operator returns an integer.
         *
         * @since 3.0
         */
        O_Round,

        /**
         * Square root. Unary arithmetic operator. The operand can be a boolean,
         * an integer or a double. This operator returns a double.
         *
         * @since 3.0
         */
        O_Sqrt,

        /**
         * Natural logarithm (base-e). Unary arithmetic operator. The operand
         * can be a boolean, an integer or a double. This operator returns a double.
         *
         * @since 3.0
         */
        O_Log,

        /**
         * Base-e exponential. Unary arithmetic operator. The operand can be a
         * boolean, an integer or a double. This operator returns a double.
         *
         * @since 3.0
         */
        O_Exp,

        /**
         * Power operator. `POW(x, y)` is equals to the value of `x` to the power of `y`.
         * The operands can be booleans, integers or doubles. This operator
         * returns a double.
         *
         * @since 3.0
         */
        O_Pow,

        /**
         * Cosine. Unary arithmetic operator. The operand can be a
         * boolean, an integer or a double. This operator returns a double.
         *
         * @since 3.0
         */
        O_Cos,

        /**
         * Sine. Unary arithmetic operator. The operand can be a
         * boolean, an integer or a double. This operator returns a double.
         *
         * @since 3.0
         */
        O_Sin,

        /**
         * Tangent. Unary arithmetic operator. The operand can be a
         * boolean, an integer or a double. This operator returns a double.
         *
         * @since 3.0
         */
        O_Tan,

        /**
         * Integer decision variable. Operator with two operands that represent
         * the lower bound and the upper bound of the decision (domain `[lb, ub]`).
         * The bounds must be integer constants.
         *
         * @since 5.0
         */
        O_Int,

        /**
         * Piecewise-linear function operator.
         * The piecewise linear function is defined by two arrays of numbers
         * giving the breakpoints of the function.
         * This operator has exactly 3 operands: The first two operands must be
         * two arrays of equal sizes (necessarily larger or equal to 2).
         * These arrays must contain constant numbers (integers or doubles). The first
         * array must contain numbers in ascending order. The third operand must
         * be an integer or a double expression. The evaluation of the piecewise
         * will fail if the value of the third operand is strictly smaller that
         * the first element of the first array, or strictly larger than the last
         * element of the first array. This operator returns a double.
         *
         * `PIECEWISE(x,y,z)` returns the image of z by the function defined by
         * geometric points `(x[0],y[0]), (x[1],y[1]), ..., (x[n-1],y[n-1])`,
         * For instance `PIECEWISE(ARRAY(0, 50, 100), ARRAY(0, 10, 100), 75)` returns `55`.
         *
         * Discontinuities are allowed in the definition of the function, that
         * is to say that two geometric points can share the same x-coordinate.
         * By convention the value taken by the function at such a discontinuous
         * point is the one associated to the last occurrence of this
         * x-coordinate in array x. For instance
         * `PIECEWISE(ARRAY(0, 50, 50, 100), ARRAY(0, 0.1, 0.9, 1), 50)` returns `0.9`;
         *
         * @since 5.0
         */
        O_Piecewise,

        /**
         * A list is an ordered collection of integers within a range `[0, n-1]` where `n`
         * is the unique argument of this operator. Mathematically a list is a
         * permutation of a subset of `[0, n-1]`. This operator takes exactly one
         * operand: a strictly positive integer constant. All values in the list
         * will be pairwise different, non negative and strictly smaller that this number.
         *
         * The elements of the list can be accessed individually with the operator {@link #O_At}.
         *
         * @since 5.5
         */
        O_List,

        /**
         * The number of elements in a list or a set. This operator takes exactly
         * one argument of type list or set and returns an integer.
         *
         * @since 5.5
         */
        O_Count,

        /**
         * The index of a value in a list (-1 if the value is not in the list).
         * This operator takes exactly two arguments: the first one is a list,
         * the second one is an integer expression.
         *
         * @since 5.5
         */
        O_IndexOf,

        /**
         * Partition. N-ary logical operator. `PARTITION(c1, c2, ..., cN)` is true
         * if all lists or sets `c1, c2, ..., cN` form a partition of their common range.
         * All the operands of this operator must be collections of the same type and
         * on the same range.
         *
         * @since 5.5
         */
        O_Partition,

        /**
         * Disjoint. N-ary logical operator. `DISJOINT(c1, c2, ..., cN)` is true if
         * all lists or sets `c1, c2, ..., cN` are pairwise disjoint. All parameters of this
         * operator must be collections of the same type and on the same range.
         *
         * @since 5.5
         */
        O_Disjoint,

        /**
         * Native function. Native functions are used to compute the value
         * of expressions from external functions written with your favorite
         * programming language. Native functions are created with the dedicated
         * method {@link LSModel#createNativeFunction}.
         *
         * @see LSNativeFunction
         * @since 6.0
         */
        O_NativeFunction,

        /**
         * Call a particular function. The first operand must be a function
         * (like {@link #O_NativeFunction} or {@link #O_Function}).
         * The other operands are passed to the function as arguments.
         * If the function is not a native function, the number of operands
         * must match the number of arguments of the function.
         *
         * @since 6.0
         */
        O_Call,

        /**
         * Function. Functions are created with the dedicated method
         * {@link LSModel#createFunction}.
         *
         * @since 7.0
         */
        O_Function,

        /**
         * Argument of a function. Arguments are automatically and implicitely
         * created when you create a function with method {@link LSModel#createFunction}.
         *
         * @since 7.0
         */
        O_Argument,

        /**
         * Range expression. This operator takes exactly two integer operands.
         * The first one is the lower bound (inclusive), the second one is the
         * upper bound (exclusive).
         *
         * A range doesn't have a value by itself but can be
         * used with N-ary operators like {@link #O_Sum}, {@link #O_Prod},
         * {@link #O_Min}, {@link #O_Max}, {@link #O_Or}, {@link #O_And},
         * {@link #O_Xor} or {@link #O_Array} to create expressions that have a
         * dynamic number of operands.
         *
         * @since 7.0
         */
        O_Range,

        /**
         * Contains. `contains(l, v)` is true if and only if the list `l` contains 
         * the value `v`. This operator takes exactly two arguments: the first 
         * one is a list, the second one is an integer expression.
         *
         * @since 7.5
         */
        O_Contains,
        
        /**
         * A set is an unordered collection of integers within a range `[0, n-1]` where `n`
         * is the unique argument of this operator. This operator takes exactly one
         * operand: a strictly positive integer constant. All values in the set
         * will be pairwise different, non negative and strictly smaller that this number.
         * Contrary to the `O_List` operator, elements in a set are not ordered and cannot be
         * indexed with `O_At`. Sets can only be manipulated with lambdas and n-ary operators like
         * `O_Sum`, `O_Min`, `O_And`, ... 
         *
         * @since 8.0
         */
        O_Set
    };
}

#endif
