// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSARRAY_H
#define LS_LSARRAY_H

#include "lsvalue.h"
#include "lsoperator.h"
#include "lsexception.h"
#include "entrypoint.h"
#include <string>

namespace localsolver {
    
    class LocalSolver;
    class LSModel;
    class BSolver;

    /**
     * Value type for array expressions. Such value is obtained with LSExpression::getArrayValue()
     * or LSSolution::getArrayValue(const LSExpression). An array contains values of type lsint,
     * lsdouble or LSArray (for multi-dimensional arrays). Note that it's possible to mix integers
     * or doubles in the same array.
     * 
     * Arrays are not decisions and cannot be modified.
     * 
     * @see LSSolution
     * @see LSExpression
     * @since 7.5
     */
    class LSArray {
    public:

        /** 
         * Returns the number of elements in the array.
         * Elements in arrays are indexed from 0 to count()-1.
         */
        int count() const {
            return ls_array_count(bsolver, barray);
        }

        /**
         * Returns true if the value at the given position is a boolean.
         * You can retrieve the value with {@link #getIntValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is a boolean.
         */
        bool isBool(int pos) const {
            return ls_array_is_bool(bsolver, barray, pos);
        }

        /**
         * Returns true if the value at the given position is an integer.
         * You can retrieve the value with {@link #getIntValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is an integer.
         */
        bool isInt(int pos) const {
            return ls_array_is_int(bsolver, barray, pos);
        }
        
        /**
         * Returns true if the value at the given position is a double.
         * You can retrieve the value with {@link #getDoubleValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is a double.
         */
        bool isDouble(int pos) const {
            return ls_array_is_double(bsolver, barray, pos);
        }
        
        /**
         * Returns true if the value at the given position is an array.
         * You can retrieve the value with {@link #getArrayValue}.
         * 
         * @param pos Position of the value to query.
         * @return True if the value at the given position is an array.
         */
        bool isArray(int pos) const {
            return ls_array_is_array(bsolver, barray, pos);
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
            return ls_array_get_int_value(bsolver, barray, pos);
        }
        
        /**
         * Returns the double value at the given position.
         * If the value is not a double, an exception is thrown.
         * 
         * @param pos Position of the value to query.
         * @return Double value.
         */
        lsdouble getDoubleValue(int pos) const {
            return ls_array_get_double_value(bsolver, barray, pos);
        }
        
        /**
         * Returns the array value at the given position.
         * If the value is not an array, an exception is thrown.
         * 
         * @param pos Position of the value to query.
         * @return Array value.
         */
        LSArray getArrayValue(int pos) const {
            return LSArray(bsolver, ls_array_get_array_value(bsolver, barray, pos));
        }

        /** 
         * Returns a string representation of the values in the array in the format
         * "{ val0, val1, ..., valN }"
         * 
         * @return String representation. 
         */
        std::string toString() const {
            int size = ls_array_to_string(bsolver, barray, NULL, 0);
            std::string str(size, '\0');
            ls_array_to_string(bsolver, barray, &str[0], size);
            return str;
        }

        ~LSArray() {}
        LSArray() { bsolver = NULL; barray = NULL; }
        LSArray(const LSArray& other) { bsolver = other.bsolver; barray = other.barray; }
        LSArray operator=(const LSArray& other) { bsolver = other.bsolver; barray = other.barray; return *this; }
        bool operator==(const LSArray& other) { return barray == other.barray && bsolver == other.bsolver; }
        bool operator!=(const LSArray& other) { return barray != other.barray || bsolver != other.bsolver; }

    private:

        friend class LocalSolver;
        friend class LSModel;
        friend class LSSolution;
        friend class LSExpression;

        LSArray(lssolver* solver, lsarray* array) : bsolver(solver), barray(array) {
            ls_check_paused_or_stopped(bsolver);
        }

        lssolver* bsolver;
        lsarray* barray;
    };
}

#endif
