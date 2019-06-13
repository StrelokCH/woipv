// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSCOLLECTION_H
#define LS_LSCOLLECTION_H

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
     * Value type for collection expressions (lists or sets).
     * Such value is obtained with LSExpression::getCollectionValue()
     * or LSSolution::getCollectionValue(const LSExpression). 
     * It represents a reference to the value of a variable and the value of
     * this variable is modified when the LSCollection object is modified.
     * 
     * @see LSSolution
     * @see LSExpression
     * @since 5.5
     */
    class LSCollection {
    public:

        /** 
         * Adds the given value to this collection.
         * Only allowed in state {@link #S_Stopped}.
         * This function will fail if the given value is outside of the domain of
         * the list/set or if this value is already included in this list/set
         * (remind that a list or a set cannot contain twice the same value).
         * 
         * @param val The value to be added. 
         */
        void add(lsint val) {
            ls_collection_add(bsolver, exprId, val);
        }

        /** 
         * Removes all values of this collection.
         * Only allowed in state {@link #S_Stopped}.
         */
        void clear() {
            ls_collection_clear(bsolver, exprId);
        }

        /** 
         * Returns the number of values in the collection.
         * Values in collections are indexed from 0 to count()-1.
         */
        int count() const {
            return (int)ls_collection_count(bsolver, exprId); 
        }

        /** 
         * Gets the value at the given position.
         * 
         * @param position The considered position (must be non negative and strictly smaller than the number of values in the collection).
         */
        lsint get(int position) const {
            return ls_collection_get(bsolver, exprId, position);
        }

        /**
         * Operator overloading for get.
         * @param position The considered position (must be non negative and strictly smaller than the number of values in the collection).
         */
        lsint operator[](int position) const {
            return get(position); 
        }

        /** 
         * Returns a string representation of the values in the collection in the format
         * "{ val0, val1, ..., valN }"
         * 
         * @return String representation. 
         */
        std::string toString() const {
            int size = ls_collection_to_string(bsolver, exprId, NULL, 0);
            std::string str(size, '\0');
            ls_collection_to_string(bsolver, exprId, &str[0], size);
            return str;
        }

        ~LSCollection() {}
        LSCollection() { bsolver = NULL; exprId = -1; }
        LSCollection(const LSCollection& other) { bsolver = other.bsolver; exprId = other.exprId; }
        LSCollection operator=(const LSCollection& other) { bsolver = other.bsolver; exprId = other.exprId; return *this; }
        bool operator==(const LSCollection& other) { return exprId == other.exprId && bsolver == other.bsolver; }
        bool operator!=(const LSCollection& other) { return exprId != other.exprId || bsolver != other.bsolver; }

    private:

        friend class LocalSolver;
        friend class LSModel;
        friend class LSSolution;
        friend class LSExpression;

        LSCollection(lssolver* solver, int exprId) : bsolver(solver), exprId(exprId) {
            ls_check_paused_or_stopped(bsolver);
            ls_expr_check_index(bsolver, exprId);
        }

        lssolver* bsolver;
        unsigned int exprId;
    };
}

#endif