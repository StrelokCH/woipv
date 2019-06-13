// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.
#ifndef LS_LSNATIVECONTEXT_H
#define LS_LSNATIVECONTEXT_H

#include "lsvalue.h"
#include "entrypoint.h"

namespace localsolver {
    
    /**
     * Context for native functions. Native context are used to query the values
     * of the arguments passed to native functions.
     *
     * @since 6.0
     */
    class LSNativeContext {
    public:

        /**
         * Returns the number of values in the current context.
         * Values in contexts are indexed from 0 to count() - 1.
         */
        int count() const {
            return ls_buffer_size(bsolver, buffer); 
        }

        /**
         * Returns true if the value at the given position is a boolean.
         * You can retrieve the value with {@link #getIntValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is a boolean.
         */
        bool isBool(int pos) const {
            return ls_buffer_is_bool(bsolver, buffer, pos);
        }

        /**
         * Returns true if the value at the given position is an integer.
         * You can retrieve the value with {@link #getIntValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is an integer.
         */
        bool isInt(int pos)  const {
            return ls_buffer_is_int(bsolver, buffer, pos);
        }

        /**
         * Returns true if the value at the given position is a double.
         * You can retrieve the value with {@link #getDoubleValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is a double.
         */
        bool isDouble(int pos) const {
            return ls_buffer_is_double(bsolver, buffer, pos);
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
            return ls_buffer_get_int_value(bsolver, buffer, pos);
        }
        
        /**
         * Returns the double value at the given position.
         * If the value is not a double, an exception is thrown.
         * 
         * @param pos Position of the value to query.
         * @return Double value.
         */
        lsdouble getDoubleValue(int pos) const {
            return ls_buffer_get_double_value(bsolver, buffer, pos);
        }
        
        ~LSNativeContext() {}
        LSNativeContext() { bsolver = NULL; buffer = NULL; }
        LSNativeContext(const LSNativeContext& other) { bsolver = other.bsolver; buffer = other.buffer; }
        LSNativeContext operator=(const LSNativeContext& other) { bsolver = other.bsolver; buffer = other.buffer; return *this; }
        bool operator==(const LSNativeContext& other) { return buffer == other.buffer && bsolver == other.bsolver; }
        bool operator!=(const LSNativeContext& other) { return buffer != other.buffer || bsolver != other.bsolver; }

    private:

        friend class LocalSolver;
        friend class LSModel;
        friend class LSSolution;
        friend class LSExpression;

        LSNativeContext(lssolver* solver, lsbuffer* buffer) : bsolver(solver), buffer(buffer) {
        }

        lssolver* bsolver;
        lsbuffer* buffer;
    };
}

#endif
