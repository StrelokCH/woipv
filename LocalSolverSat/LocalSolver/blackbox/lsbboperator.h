// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXOPERATOR_H
#define LS_BLACKBOXOPERATOR_H

namespace localsolverblackbox {

    /**
     * Mathematical operators available for modeling. These operators are used to type 
     * the expressions created in LocalSolverBlackBox mathematical optimization model.
     * 
     * @see LSBBModel
     * @see LSBBExpression 
     */
    enum LSBBOperator {
        /** 
         * Boolean decision. Decisional operator with no operand. 
         * Decision variable with domain {0,1}. 
         */
        O_Bool,
        
        /** 
         * Integer decision variable. Decisional operator with two operands min and max.
         * Decision variable with domain [min,max].
         */
        O_Int,
        
        /**
         * Float decision. Decisional operator with two operands min and max.
         * Decision variable with domain [min,max].
         */
        O_Float,
        
        /** 
         * Constant. Unary operator. Can be equal to any integer. 
         * Note that constants 0 or 1 are considered as boolean.
         */
        O_Const,
        
        /**
         * Native function. Native functions are used to retrieve the value
         * of expressions from external functions written with your favorite
         * language. Native functions are created with the dedicated
         * method {@link LSBBModel#createNativeFunction}.
         * @see LSBBNativeFunction
         */
        O_NativeFunction,
        
        /**
         * Call a particular function. The first operand must be a function
         * (like O_NativeFunction). The other operands are passed to the
         * function as arguments.
         */
        O_Call
    };
}

#endif

