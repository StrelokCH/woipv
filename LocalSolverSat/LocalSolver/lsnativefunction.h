// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.
#ifndef LS_LSNATIVEFUNCTION_H
#define LS_LSNATIVEFUNCTION_H

#include "lsnativecontext.h"
#include "lsvalue.h"

namespace localsolver {

    /**
     * Native function interface. To use your own native functions within %LocalSolver,
     * you have to first define your native function in one of two ways:
     * <ul>
     *  <li>Implement the LSNativeFunction interface with the call method. The
     *      call method must take the native context associated to the function
     *      and must return a double value. This native context
     *      contains the values of the expressions passed to the function.
     *      A distinction is made between integer arguments (bool, int) and
     *      floating point arguments (double)</li>
     *  <li>If your program is compiled with C++11, you can use a function, a functor
     *      or a lambda function instead. This function may take any number of lsint
     *      and lsdouble arguments, or alternatively a single native context. It must
     *      return a double value.</li>
     * </ul>
     * 
     * Then:
     * <ol>
     *  <li>Instanciate the function as an LSExpression with
     *      {@link LSModel#createNativeFunction} or the dedicated shortcut
     *      {@link LSModel#nativeFunction}.</li>
     *  <li>Pass arguments to your function and call it using {@link LSModel#call}.
     *      The first operand must the LSExpression returned by createNativeFunction.
     *      The other operands must be LSExpressions, whose values will be made accessible
     *      to your native function when it is called.</li>
     * </ol>
     *
     * <strong>Note 1:</strong> Most of the time your native function will be called
     * when the solver is in state {@link #S_Running}.
     * Do not attempt to call any method of the solver (to retrieve statistics,
     * values of LSExpressions or whatever) in that state or an exception will
     * be thrown. The only accessible function is {@link LocalSolver#stop()}.
     *
     * <strong>Note 2:</strong> Your functions must be thread-safe. According to the
     * "nbThreads" parameter, %LocalSolver can be multi-threaded. In that case, your
     * native functions must be thread safe. If you cannot guarantee the thread-safety
     * of your code, we strongly recommend you to limit the search of %LocalSolver
     * to one thread with {@link LSParam#setNbThreads}.
     *
     * <strong>Note 3:</strong> %LocalSolver does not manage memory of objects
     * created outside of its environment. You are responsible for the lifetime of
     * your LSNativeFunction, which must last as long as the search is active.
     *
     * @since 6.0
     */
    class LSNativeFunction {
    public:

        /**
         * The function to call. The native context contains the arguments
         * to pass to your function.
         *
         * @param context Native context containing the arguments of the function.
         * @return The value of the LSExpression.
         */
        virtual lsdouble call(const LSNativeContext& context) = 0;

        virtual ~LSNativeFunction() {}
    };
}

#endif
