// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSINCONSISTENCY_H
#define LS_LSINCONSISTENCY_H

#include "lsexpression.h"
#include "lsexception.h"
#include "entrypoint.h"

namespace localsolver {
    class LocalSolver;

    /**
     * Inconsistency core of the optimization model. Also known as Irreductible Inconsistent Subproblem. 
     */
    class LSInconsistency {
    public:

       /** 
         * Gets the number of expressions responsible for the inconsistency of this model.
         * The sub problem reduced to these expressions and their operands (recursively) is inconsistent.
         * This method returns 0 if the inconsistency of the problem could not be proven. 
         * @return Number of expressions.
         */
        int getNbCauses() const {
            return ls_nb_causes(bsolver);
        }
        
        /** 
         * Gets the cause with the given index in this model.
         * 
         * @param causeIndex A non negative integer strictly smaller than getNbCauses().
         * @return The requested cause (Expression). 
         */
        LSExpression getCause(int causeIndex) const {            
            return LSExpression(bsolver, ls_cause(bsolver, causeIndex));
        }
        
        /** 
         * Returns a string representation of this inconsistency core. 
         * @return String representation. 
         */
        std::string toString() const {
            int size = ls_inconsistency_to_string(bsolver, NULL, 0);
            std::string str(size, '\0');
            ls_inconsistency_to_string(bsolver, &str[0], size);
            return str;
        }

        ~LSInconsistency() {}
        LSInconsistency() { bsolver = NULL; }
        LSInconsistency(const LSInconsistency& other) { bsolver = other.bsolver; }
        LSInconsistency& operator=(const LSInconsistency& other) { bsolver = other.bsolver; return *this; }
        
    private:
        friend class LocalSolver;

        explicit LSInconsistency(lssolver* solver) : bsolver(solver) {
            ls_check_paused_or_stopped(bsolver);
        }

        lssolver* bsolver;
    };
}

#endif
