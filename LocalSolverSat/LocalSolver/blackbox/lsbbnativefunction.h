// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXNATIVEFUNCTION_H
#define LS_BLACKBOXNATIVEFUNCTION_H

#include "lsbbnativecontext.h"
#include "lsbbvalue.h"

namespace localsolverblackbox {

    /**
     * Native function interface. To use your own functions with LocalSolverBlackBox,
     * you have to proceed in 3 steps:
     * <ol>
     *  <li>Implement the LSBBNativeFunction interface with the call method. The
     *      call method must take the native context associated to the function
     *      and must return a double value. This native context
     *      contains the values of the expressions passed to the function.
     *      A distinction is made between integer arguments (bool, int) and
     *      floating point arguments (double).</li>
     *  <li>Instanciate the function as an LSBBExpression with
     *      {@link LSBBModel#createNativeFunction}.</li>
     *  <li>Pass arguments to your function and call it. For that, you have to
     *      create expressions of type {@link #O_Call}. The first operand must
     *      be your native function. The other operands must be LSBBExpressions.
     *      Their value will be made accessible to your native function through
     *      the native context.</li>
     *  </ol>
     *
     * <strong>Note 1:</strong> Most of the time your native function will be called
     * when the solver is in state {@link #S_Running}. 
     * Do not attempt to call any method of the solver (to retrieve statistics,
     * values of LSBBExpressions or whatever) in that state or an exception will
     * be thrown. The only accessible function is {@link LocalSolverBlackBox#stop()}.
     *
     * <strong>Note 2:</strong> You have to free the memory of the native functions you created.
     * LocalSolverBlackBox does not manage memory of objects created outside of its environment.
     */
    class LSBBNativeFunction {
    public:

        /**
         * The function to call. The native context contains the arguments
         * to pass to your function.
         *
         * @param context Native context containing the arguments of the function.
         * @return The value of the LSBBExpression.
         */
        virtual lsdouble call(const LSBBNativeContext& context) = 0;
        
        
    };
}

#endif