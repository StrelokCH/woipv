// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSSTATISTICS_H
#define LS_LSSTATISTICS_H

#include "entrypoint.h"
#include <string>

namespace localsolver {

    class LocalSolver;
    class LSPhase;

    /**
     * Statistics of the search. The statistics are useful for evaluating the quality of your model. 
     * Indeed, your model is suited for %LocalSolver (and more generally for local search) if the 
     * percentage of accepted moves is larger than 1 % with an annealing level equal to 0. Larger is 
     * the percentage of accepted moves, larger is the diversification of the search, better is 
     * the convergence toward high-quality solutions. If the number of accepted moves is too low, 
     * we suggests you to relax some constraints of the model (goal programming) or to augment the 
     * annealing level. A fine modeling is much preferable to a fine tuning of the search, both for 
     * effectiveness and for robustness.
     */
    class LSStatistics {
    public:

        /** 
         * Gets the running time in seconds. Note that is the "real" time spent (and not 
         * only the CPU or system time).
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         *
         * @return Running time in seconds. 
         */
        int getRunningTime() const {
            return ls_stats_get_running_time(bsolver, phaseId);
        }

        /** 
         * Gets the number of iterations.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of iterations. 
         */
        long long getNbIterations() const {
            return ls_stats_get_nb_iterations(bsolver, phaseId);
        }

        /** 
         * Gets the number of (attempted) moves. This corresponds to the number of explored 
         * solutions (feasible or infeasible).
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of (attempted) moves. 
         */
        long long getNbMoves() const {
            return ls_stats_get_nb_moves(bsolver, phaseId);
        }

        /** 
         * Gets the number of accepted moves.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of accepted moves. 
         */
        long long getNbAcceptedMoves() const {
            return ls_stats_get_nb_accepted_moves(bsolver, phaseId);
        }

        /** 
         * Gets the number of accepted moves having improved the cost of the current solution. 
         * Improving moves are a part of accepted moves.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of improving moves.
         */
        long long getNbImprovingMoves() const {
            return ls_stats_get_nb_improving_moves(bsolver, phaseId);
        }

        /** 
         * Gets the number of rejected moves.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of rejected moves.
         */
        long long getNbRejectedMoves() const {
            return ls_stats_get_nb_rejected_moves(bsolver, phaseId);
        }

        /** 
         * Gets the number of moves leading to infeasible solutions. Infeasible moves are a part 
         * of rejected moves.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of infeasible moves.
         */
        long long getNbInfeasibleMoves() const {
            return ls_stats_get_nb_infeasible_moves(bsolver, phaseId);
        }

        /** 
         * Gives the percentage of accepted moves relative to the number of moves.  
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Percentage of accepted moves.
         */
        double getPercentAcceptedMoves() const {
            return ls_stats_get_percent_accepted_moves(bsolver, phaseId);
        }

        /** 
         * Gives the percentage of improving moves relative to the number of moves.  
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Percentage of improving moves.
         */
        double getPercentImprovingMoves() const {
            return ls_stats_get_percent_improving_moves(bsolver, phaseId);
        }

        /** 
         * Gives the percentage of rejected moves relative to the number of moves.  
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Percentage of rejected moves.
         */
        double getPercentRejectedMoves() const {
            return ls_stats_get_percent_rejected_moves(bsolver, phaseId);
        }

        /** 
         * Gives the percentage of infeasible moves relative to the number of moves.  
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Percentage of infeasible moves.
         */
        double getPercentInfeasibleMoves() const {
            return ls_stats_get_percent_infeasible_moves(bsolver, phaseId);
        }

        /** 
         * Returns a string representation of the statistics. Only allowed in states 
         * {@link #S_Paused} or {@link #S_Stopped}. Useful for debugging or logging purposes. 
         * 
         * @return String representation. 
         */
        std::string toString() const {
            int size = ls_stats_to_string(bsolver, phaseId, NULL, 0);
            std::string str(size, '\0');
            ls_stats_to_string(bsolver, phaseId, &str[0], size);
            return str;
        }

        /**
         * Returns useful info about the search while running. Only allowed if the solver has been started at least once. 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Useful for debugging or logging purposes.
         *
         * Here are some explanations about the output string:
         * - "mov" corresponds to the number of moves performed.
         * - "inf" corresponds to the percentage of infeasible moves.
         * - "acc" corresponds to the percentage of accepted moves.
         * - "imp" corresponds to the number of improving moves. 
         * 
         * @return Info about the search while running.
         */
        std::string getInfo() const {
            int size = ls_stats_info(bsolver, phaseId, NULL, 0);
            std::string str(size, '\0');
            ls_stats_info(bsolver, phaseId, &str[0], size);
            return str;
        }

        ~LSStatistics() {}
        LSStatistics(const LSStatistics& other) { bsolver = other.bsolver; phaseId = other.phaseId; }
        LSStatistics& operator=(const LSStatistics& other) { bsolver = other.bsolver; phaseId = other.phaseId; return *this; }
        
    private:

        friend class LocalSolver;
        friend class LSPhase;

        LSStatistics(lssolver* solver, int phaseId) : bsolver(solver), phaseId(phaseId) {
            ls_check_paused_or_stopped(bsolver);
        }

        lssolver* bsolver;
        unsigned int phaseId;
    };
}

#endif
