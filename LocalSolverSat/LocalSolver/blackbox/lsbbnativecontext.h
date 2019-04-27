// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXNATIVECONTEXT_H
#define LS_BLACKBOXNATIVECONTEXT_H

#include "lsbbvalue.h"
#include "bbentrypoint.h"
#include <cstdlib>

namespace localsolverblackbox {
    class BBSolver;
    class BBExternalBuffer;

    /**
     * Context for native functions. Native context are used to query the values
     * of the arguments passed to native functions.
     */
    class LSBBNativeContext {
    public:

        /**
         * Returns the number of values in the current context.
         */
        int count() const {
            return lsbb_buffer_size(bbsolver, buffer);
        }

        /**
         * Returns true if the value at the given position is a boolean.
         * You can retrieve the value with {@link #getIntValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is a boolean.
         */
        bool isBool(int pos) const {
            return lsbb_buffer_is_bool(bbsolver, buffer, pos);
        }

        /**
         * Returns true if the value at the given position is an integer.
         * You can retrieve the value with {@link #getIntValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is an integer.
         */
        bool isInt(int pos) const {
            return lsbb_buffer_is_int(bbsolver, buffer, pos);
        }

        
        /**
         * Returns true if the value at the given position is a double.
         * You can retrieve the value with {@link #getDoubleValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is a double.
         */
        bool isDouble(int pos) const {
            return lsbb_buffer_is_double(bbsolver, buffer, pos);
        }

        /**
         * Returns the integer value at the given position.
         * If the value is neither an integer nor a boolean, an exception
         * is thrown.
         * 
         * @param pos Position of the value to query.
         * @return Integer value.
         */
        lsint getIntValue(int pos) const {
            return lsbb_buffer_get_int_value(bbsolver, buffer, pos);
        }

        /**
         * Returns the double value at the given position.
         * If the value is not a double, an exception is thrown.
         * 
         * @param pos Position of the value to query.
         * @return Double value.
         */
        lsdouble getDoubleValue(int pos) const {
            return lsbb_buffer_get_double_value(bbsolver, buffer, pos);
        }

        ~LSBBNativeContext() {
        }

        bool operator==(const LSBBNativeContext& other) {
            return buffer == other.buffer && bbsolver == other.bbsolver;
        }

        bool operator!=(const LSBBNativeContext& other) {
            return buffer != other.buffer || bbsolver != other.bbsolver;
        }

        LSBBNativeContext(BBSolver* solver, BBExternalBuffer* buffer) : bbsolver(solver), buffer(buffer) {
        }


    private:
        friend class LocalSolverBlackBox;
        friend class LSBBModel;
        friend class LSBBSolution;
        friend class LSBBExpression;


        BBSolver* bbsolver;
        BBExternalBuffer* buffer;
    };
}

#endif
