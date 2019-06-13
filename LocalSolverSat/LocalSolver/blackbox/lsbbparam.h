// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXPARAM_H
#define LS_BLACKBOXPARAM_H

#include "bbentrypoint.h"
#include "lsbbvalue.h"

namespace localsolverblackbox{
    
    /**
     * Solving parameters. This class contains some methods allowing you to parameterize 
     * the resolution of the model. For the sake of simplicity, only a few parameters are 
     * actually offered to tune the search.
     */
    class LSBBParam{
    public:
        
        /** 
         * Sets the seed of the pseudo-random number generator used by the solver. 
         * The seed must be a positive integer. The default seed is set to 0. 
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param seed Seed of the pseudo-random number generator.
         */
        void setSeed(int seed) {
            lsbb_set_seed(bsolver, seed);
        }
        
        /** 
         * Gets the seed of the pseudo-random number generator used by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Seed of the pseudo-random number generator.
         * @see #setSeed(int)
         */
        int getSeed() const{
            return lsbb_get_seed(bsolver);
        }
        
        
        /**
        * Sets the time limit in seconds. Note that the "real" time
        * (that is, total time) spent to resolve the model is considered here (and
        * not only the CPU time). The default time limit is set to the largest
        * positive integer on 32 bits, that is 2^31-1 = 2,147,483,647 &gt; 10^9.
        * Only allowed in state {@link #S_Stopped}.
        *
        * @param limit Time limit in seconds.
        */
        void setTimeLimit(int limit){
            lsbb_set_time_limit(bsolver, limit);
        }
        
        /**
        * Gets the time limit in seconds. Only allowed in states
        * {@link #S_Paused} or {@link #S_Stopped}.
        *
        * @return Time limit in seconds.
        * @see #setTimeLimit(int)
        */
        int getTimeLimit() const{
            return lsbb_get_time_limit(bsolver);
        }
        
        /**
        * Sets the maximum number of blackbox evaluations. 
        * The default number of evaluations is set to the largest positive integer on 32 bits
        * that is 2^31-1 = 2,147,483,647 &gt; 10^9.
        * Only allowed in state {@link #S_Stopped}.
        *
        * @param limit Maximum number of blackbox evaluation.
        */
        void setEvaluationLimit(int limit){
            lsbb_set_evaluation_limit(bsolver, limit);
        }

        /**
        * Gets the maximum number of blackbox evalutions.
        * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
        *
        * @return Evaluation limit.
        * @see #setEvaluationLimit(int)
        */        
        int getEvaluationLimit() const{
            return lsbb_get_evaluation_limit(bsolver);
        }
        
        /** 
         * Sets the objective bound. If the objective is minimized 
         * (resp. maximized), then the optimization of this objective is stopped as soon as this 
         * lower (resp. upper) bound is reached. 
         * Only allowed in states {@link #S_Stopped}.
         * 
         * @param bound Objective bound.
         */
        void setObjectiveBound(double bound){
            lsbb_set_objective_bound(bsolver, bound);
        }
        
        ~LSBBParam() {}
        LSBBParam(const LSBBParam& other) { bsolver = other.bsolver; }
        LSBBParam& operator=(const LSBBParam& other) { bsolver = other.bsolver; return *this; }
        
        
    private:
        friend class LocalSolverBlackBox;
        
        explicit LSBBParam(lsbbsolver* solver) : bsolver(solver) {
        }
        
        
        lsbbsolver* bsolver;
        
    };
}

#endif
