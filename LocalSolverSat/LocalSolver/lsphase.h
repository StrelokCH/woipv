// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_PHASE_H
#define LS_PHASE_H

#include "lsvalue.h"
#include "entrypoint.h"
#include "lsstatistics.h"
#include <string>

namespace localsolver {
    class LocalSolver;

    /**
     * Optimization phase. Since %LocalSolver allows lexicographic multiobjective optimization, 
     * we allow you to parameterize the time or the number of iterations to spend in optimizing 
     * each objective. When multiple objectives are declared, these objectives are optimized in 
     * lexicographic order (in the order of their declaration in the model). Thus, if <em>k</em> 
     * objectives <em>0, ..., k-1</em> are declared, each objective <em>i</em> can be optimized 
     * while maintaining the values of objectives <em>0, ..., i-1</em> previously obtained and 
     * ignoring the values of objectives <em>i+1, ..., k-1</em>. During such an optimization 
     * phase, objective <em>i</em> is called the optimized objective.  
     * 
     * At least one phase must be created to launch the solver. If you have only one objective to 
     * optimize, then create a phase, set the time or the number of iterations, and run the solver. 
     * By default, the optimized objective of a phase corresponds to the last objective declared in 
     * the model (in this case, all objectives of the model are optimized together).
     */
    class LSPhase {
    public:

        /** 
         * Sets the time limit of this phase in seconds. Note that the "real" time (that is, 
         * total time) spent to resolve the model is considered here (and not only the CPU 
         * time). The default time limit is set to the largest positive integer on 32 bits, 
         * that is 2^31-1 = 2,147,483,647 &gt; 10^9.
         * Only allowed in state {@link #S_Stopped}.
         *
         * Note that if the optimal value of an objective is found before the end of the
         * corresponding phase, the remaining time will be transferred to the next phase.
         * 
         * @param timeLimit Time limit in seconds.
         */
        void setTimeLimit(int timeLimit) {
            ls_phase_set_time_limit(bsolver, phaseIndex, timeLimit);
        }

        /** 
         * Gets the time limit of this phase in seconds. 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Time limit in seconds.
         * @see #setTimeLimit(int)
         */
        int getTimeLimit() const {
            return ls_phase_get_time_limit(bsolver, phaseIndex);
        }

        /** 
         * Sets the number of iterations of this phase. Fixing the seed and the number 
         * of iterations of all phases ensures the reproducibility of results over several runs.
         * The default number of iterations is set to the largest positive integer on 64 bits, 
         * that is 2^63-1 = 9,223,372,036,854,775,807 &gt; 10^18.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * Note that if the optimal value of an objective is found before the end of the
         * corresponding phase, the remaining iterations will be transferred to the next phase.
         *
         * @param iterationLimit Iteration limit.
         */
        void setIterationLimit(long long iterationLimit) {
            ls_phase_set_iteration_limit(bsolver, phaseIndex, iterationLimit);
        }

        /** 
         * Gets the number of iterations of this phase. 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Iteration limit.
         * @see #setIterationLimit
         */
        long long getIterationLimit() const {
            return ls_phase_get_iteration_limit(bsolver, phaseIndex);
        }

        /**
         * Sets the optimized objective of this phase. When multiple objectives are declared, 
         * these objectives are optimized in lexicographic order (in the order of their declaration 
         * in the model). Thus, if <em>k</em> objectives <em>0, ..., k-1</em> are declared, each 
         * objective <em>i</em> can be optimized while maintaining the values of objectives 
         * <em>0, ..., i-1</em> previously obtained and ignoring the values of objectives
         * <em>i+1, ..., k-1</em>. By default, the optimized objective corresponds to the last 
         * objective declared in the model. Only allowed in state {@link #S_Stopped}.
         * 
         * @param objectiveIndex Index of the optimized objective.
         */
        void setOptimizedObjective(int objectiveIndex) {
            ls_phase_set_optimized_objective(bsolver, phaseIndex, objectiveIndex);
        }

        /**
         * Gets the index of the optimized objective. 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Index of the optimized objective.
         * @see #setOptimizedObjective(int)
         */
        int getOptimizedObjective() const {
            return ls_phase_get_optimized_objective(bsolver, phaseIndex);
        }

        /**
         * Sets the enabled status of this phase. The solver only optimize enabled phases.
         * Disabling a phase can be useful when an objective has been removed from the model by {@link LSModel#removeObjective}.
         * By default, a phase is enabled. Only allowed in state {@link #S_Stopped}.
         * 
         * @param enabled True to enable the phase, False to disable it
         * @since 5.0
         */
        void setEnabled(bool enabled) {
            ls_phase_set_enabled(bsolver, phaseIndex, enabled);
        }
        
        /**
         * Returns true if the phase is enabled, False otherwise.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return True if phase is enabled 
         * @since 5.0
         */
        bool isEnabled() const {
            return ls_phase_is_enabled(bsolver, phaseIndex);
        }
        
        /** 
         * Gets the statistics of this phase. Statistics are reset to zero before each resolution. 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}. Note that for performance reasons, 
         * this function always returns the same object.
         * 
         * @return Statistics.
         * @see LSStatistics
         */
        LSStatistics getStatistics() const {
            return LSStatistics(bsolver, phaseIndex);
        }

        /** 
         * Returns a string representation of this phase. 
         * Useful for debugging or logging purposes. 
         * 
         * @return String representation. 
         */
        std::string toString() const {
            int size = ls_phase_to_string(bsolver, phaseIndex, NULL, 0);
            std::string str(size, '\0');
            ls_phase_to_string(bsolver, phaseIndex, &str[0], size);
            return str;
        }
        
        ~LSPhase() {}
        LSPhase() { bsolver = NULL; phaseIndex = -1; }
        LSPhase(const LSPhase& other) { bsolver = other.bsolver; phaseIndex = other.phaseIndex; }
        LSPhase& operator=(const LSPhase& other) { bsolver = other.bsolver; phaseIndex = other.phaseIndex; return *this; }

    private:
        friend class LocalSolver;
        
        lssolver* bsolver;
        int phaseIndex;

        LSPhase(lssolver* solver, int phaseIndex) : bsolver(solver), phaseIndex(phaseIndex) {
            ls_check_paused_or_stopped(bsolver);
            ls_phase_check_index(bsolver, phaseIndex);
        }
    };
}

#endif
