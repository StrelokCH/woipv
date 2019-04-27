// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSPARAM_H
#define LS_LSPARAM_H

#include "lsvalue.h"
#include "entrypoint.h"
#include <string>

namespace localsolver {
    class LocalSolver;
    
    /**
     * Solving parameters. This class contains some methods allowing you to parameterize 
     * the resolution of the model. For the sake of simplicity, only a few parameters are 
     * actually offered to tune the search.
     */
    class LSParam {
    public:

        /** 
         * Sets the seed of the pseudo-random number generator used by the solver. 
         * The seed must be a positive integer. The default seed is set to 0. 
         * Only allowed in state {@link #S_Stopped}.
         * 
         * The search for solutions is highly randomized. Fixing the seed and the 
         * number of iterations of the solver allows you to reproduce exactly its 
         * results over several runs.
         * 
         * @param seed Seed of the pseudo-random number generator.
         */
        void setSeed(int seed) {
            ls_set_seed(bsolver, seed);
        }

        /** 
         * Gets the seed of the pseudo-random number generator used by the solver.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Seed of the pseudo-random number generator.
         * @see #setSeed(int)
         */
        int getSeed() const {
            return ls_get_seed(bsolver);
        }

        /** 
         * Sets the number of threads used to parallelize the search. The number of threads must be a strictly 
         * positive integer. The default number of threads is set to 2. Only allowed in state {@link #S_Stopped}. 
         * 
         * By increasing the number of threads, you increase the robustness of the solver (that is, the 
         * chance to find better solutions). However, we recommend you to avoid running a number of threads 
         * which exceeds the number of cores available on your machine. Since the performance of the solver 
         * is sensible to the number of cache misses, the recommended ratio for maximizing its efficiency is 
         * to launch at most <em>k</em>/2 searches with <em>k</em> the number of cores of your hardware.
         * 
         * @param nbThreads Number of threads.
         */
        void setNbThreads(int nbThreads) {
            ls_set_nb_threads(bsolver, nbThreads);
        }

        /**
         * Gets the number of threads. Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of threads.
         * @see #setNbThreads(int)
         */
        int getNbThreads() const {
            return ls_get_nb_threads(bsolver);
        }

        /** 
         * Sets the simulated annealing level. The level must be an integer between 0 and 9.
         * The default simulated annealing level is set to 1. Only allowed in state {@link #S_Stopped}.
         * 
         * If set to 0, the search heuristic is equivalent to a standard descent: moves deteriorating the current 
         * solution are rejected. By increasing this parameter, you increase the number of uphill moves (that is, 
         * moves deteriorating the objective value of the current solution): this increases chances to reach better 
         * solutions (diversification), but slows the convergence of the search. 
         * 
         * @param annealingLevel Simulated annealing level.
         */
        void setAnnealingLevel(int annealingLevel) {
            ls_set_annealing_level(bsolver, annealingLevel);
        }

        /** 
         * Gets the simulated annealing level.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Simulated annealing level.
         * @see #setAnnealingLevel(int)
         */
        int getAnnealingLevel() const {
            return ls_get_annealing_level(bsolver);
        }

        /**
         * Sets the verbosity level of the solver. The default verbosity is set to 1.
         * There are 3 defined verbosity levels :
         * <ul>
         *   <li>0 : All the traces are disabled.</li>
         *   <li>1 : Normal verbosity. This is the default level.</li>
         *   <li>2 : Detailed verbosity. Displays statistics during the search.</li>
         * </ul>
         * 
         * @param verbosity Verbosity level: 0, 1, 2.
         */
        void setVerbosity(int verbosity) {
            ls_set_verbosity(bsolver, verbosity);
        }

        /**
         * Gets the verbosity level of the solver.
         * 
         * @return verbosity Verbosity level: 0, 1 or 2.
         * @see #setVerbosity(int)
         */
        int getVerbosity() const {
            return ls_get_verbosity(bsolver);
        }

        /** 
         * Sets the bound of the objective with the given index. If the objective is minimized 
         * (resp. maximized), then the optimization of this objective is stopped as soon as this 
         * lower (resp. upper) bound is reached. It can be useful for goal programming. 
         * Only allowed in states {@link #S_Stopped}.
         * This method has the same behavior as {@link #setIntObjectiveBound}.
         * 
         * @param objectiveIndex Index of the objective.
         * @param bound Objective bound.
         * @see LSExpression#isObjective()
         * @see LSExpression#isInt()
         */
        void setObjectiveBound(int objectiveIndex, lsint bound) {
            ls_set_int_objective_bound(bsolver, objectiveIndex, bound);
        }
        
        /** 
         * Sets the bound of the objective with the given index. If the objective is minimized 
         * (resp. maximized), then the optimization of this objective is stopped as soon as this 
         * lower (resp. upper) bound is reached. It can be useful for goal programming.
         * Only allowed in states {@link #S_Stopped}. 
         * This method has the same behavior as {@link #setDoubleObjectiveBound}.
         * 
         * @param objectiveIndex Index of the objective.
         * @param bound Objective bound.
         * @see LSExpression#isObjective()
         * @see LSExpression#isDouble()
         * @since 3.1
         */
        void setObjectiveBound(int objectiveIndex, lsdouble bound) {
            ls_set_double_objective_bound(bsolver, objectiveIndex, bound);
        }
        
        /** 
         * Sets the bound of the objective with the given index. If the objective is minimized 
         * (resp. maximized), then the optimization of this objective is stopped as soon as this 
         * lower (resp. upper) bound is reached. It can be useful for goal programming. 
         * Only allowed in states {@link #S_Stopped}.
         * 
         * @param objectiveIndex Index of the objective.
         * @param bound Objective bound.
         * @see LSExpression#isObjective()
         * @see LSExpression#isInt()
         * @since 3.0
         */
        void setIntObjectiveBound(int objectiveIndex, lsint bound) {
            ls_set_int_objective_bound(bsolver, objectiveIndex, bound);
        }
        
        /** 
         * Sets the bound of the objective with the given index. If the objective is minimized 
         * (resp. maximized), then the optimization of this objective is stopped as soon as this 
         * lower (resp. upper) bound is reached. It can be useful for goal programming.
         * Only allowed in states {@link #S_Stopped}.
         * 
         * @param objectiveIndex Index of the objective.
         * @param bound Objective bound.
         * @see LSExpression#isObjective()
         * @see LSExpression#isDouble()
         * @since 3.0
         */
        void setDoubleObjectiveBound(int objectiveIndex, lsdouble bound) {
            ls_set_double_objective_bound(bsolver, objectiveIndex, bound);
        }

        /** 
         * Gets the bound of the objective with the given index.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param objectiveIndex Index of the objective.
         * @return Objective bound.
         * @see #setObjectiveBound(int, lsint)
         */
        lsint getObjectiveBound(int objectiveIndex) const {
            return ls_get_int_objective_bound(bsolver, objectiveIndex);
        }
        
        /** 
         * Gets the bound of the objective with the given index.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param objectiveIndex Index of the objective.
         * @return Objective bound.
         * @see #setObjectiveBound(int, lsint)
         * @since 3.O
         */
        lsint getIntObjectiveBound(int objectiveIndex) const {
            return ls_get_int_objective_bound(bsolver, objectiveIndex);
        }
        
        /** 
         * Gets the bound of the objective with the given index.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param objectiveIndex Index of the objective.
         * @return Objective bound.
         * @see #setObjectiveBound(int, lsint)
         * @since 3.0
         */
        lsdouble getDoubleObjectiveBound(int objectiveIndex) const {
            return ls_get_double_objective_bound(bsolver, objectiveIndex);
        }

        /** 
         * Sets the time in seconds between two consecutive displays in console while the solver is running.
         * The default time between displays is set to 1 second. Only allowed in state {@link #S_Stopped}.
         * 
         * @param timeBetweenDisplays Time in seconds between displays.
         */
        void setTimeBetweenDisplays(int timeBetweenDisplays) {
            ls_set_time_between_displays(bsolver, timeBetweenDisplays);
        }

        /** 
         * Gets the time in seconds between two consecutive displays in console while the solver is running.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Time in seconds between displays.
         * @see #setTimeBetweenDisplays(int)
         */
        int getTimeBetweenDisplays() const {
            return ls_get_time_between_displays(bsolver);
        }
        
        /** 
         * Sets the time in seconds between two events of type {@link #CT_TimeTicked}.
         * The default time between ticks is set to 1 second. Only allowed in state {@link #S_Stopped}.
         * 
         * @param timeBetweenTicks Time in seconds between ticks.
         * @since 6.0
         */
        void setTimeBetweenTicks(int timeBetweenTicks) {
            ls_set_time_between_ticks(bsolver, timeBetweenTicks);
        }

        /** 
         * Gets the time in seconds between two events of type {@link #CT_TimeTicked}.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Time in seconds between ticks.
         * @see #setTimeBetweenTicks(int)
         * @since 6.0
         */
        int getTimeBetweenTicks() const {
            return ls_get_time_between_ticks(bsolver);
        }
        
        /** 
         * Sets the number of iterations between two events of type {@link #CT_IterationTicked}.
         * The default number of iterations between ticks is set to 10,000. Only allowed in state {@link #S_Stopped}.
         * 
         * @param iterationBetweenTicks Number of iterations between ticks.
         * @since 6.0
         */
        void setIterationBetweenTicks(long long iterationBetweenTicks) {
            ls_set_iteration_between_ticks(bsolver, iterationBetweenTicks);
        }

        /** 
         * Gets the number of iterations between two events of type {@link #CT_IterationTicked}.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of iterations between ticks.
         * @see #setIterationBetweenTicks(long long)
         * @since 6.0
         */
        long long getIterationBetweenTicks() const {
            return ls_get_iteration_between_ticks(bsolver);
        }
        
        /**
         * Sets the path of the %LocalSolver log file. If the path is empty, no log
         * will be saved. To tune the logging verbosity, see {@link #setVerbosity}.
         * Only allowed in states {@link #S_Stopped} or {@link #S_Modeling}.
         * 
         * @param path Path of the log file. Leave empty to disable the logging mechanism.
         * 
         * @see #setVerbosity(int)
         * @see #getLogFile()
         */
        void setLogFile(const std::string& path) {
            ls_set_log_file(bsolver, path.c_str());
        }
        
        /**
         * Returns the path of the %LocalSolver log file. If no log file was
         * specified, an empty string is returned.
         * 
         * @return The path of the log file or an empty string.
         * @see #setLogFile
         */
        std::string getLogFile() const {
            int size = ls_get_log_file(bsolver, NULL, 0);
            std::string str(size, '\0');
            ls_get_log_file(bsolver, &str[0], size);
            return str;
        }

        /**
         * Sets the value of an advanced parameter. Advanced parameters are reserved 
         * for fine tuning or debugging and should not be used by end-users. Only allowed 
         * in states {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @param value Value of the parameter.
         */
        void setAdvancedParam(const std::string& key, int value) {
            ls_set_advanced_param(bsolver, key.c_str(), value);
        }
        
        /**
         * Sets the value of an advanced parameter. Advanced parameters are reserved 
         * for fine tuning or debugging and should not be used by end-users. Only allowed 
         * in states {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @param value Value of the parameter.
         */
        void setAdvancedParam(const std::string& key, double value) {
            ls_set_advanced_double_param(bsolver, key.c_str(), value);
        }
        
        /**
         * Sets the value of an advanced parameter. Advanced parameters are reserved 
         * for fine tuning or debugging and should not be used by end-users. Only allowed 
         * in states {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @param value Value of the parameter.
         */
        void setAdvancedParam(const std::string& key, const std::string& value) {
            ls_set_advanced_str_param(bsolver, key.c_str(), value.c_str());
        }

        /**
         * Returns the value of an advanced parameter. Advanced parameters are reserved for 
         * fine tuning or debugging and should not be used by end-users. Throws an exception 
         * if the parameter does not exist. Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @return Value of the parameter.
         * @see #setAdvancedParam(const std::string&, int)
         */
        int getAdvancedParam(const std::string& key) const {
            return ls_get_advanced_param(bsolver, key.c_str(), 0);
        }

        /**
         * Returns the value of an advanced parameter. Advanced parameters are reserved for 
         * fine tuning or debugging and should not be used by end-users. The default value is 
         * returned if the parameter does not exist or if no value is defined for this parameter.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @param defaultValue Default value.
         * @see #setAdvancedParam(const std::string&, int)
         */
        int getAdvancedParam(const std::string& key, int defaultValue) const {
            return ls_get_advanced_param(bsolver, key.c_str(), defaultValue);
        }
        
        /**
         * Returns the value of an advanced parameter. Advanced parameters are reserved for 
         * fine tuning or debugging and should not be used by end-users. The default value is 
         * returned if the parameter does not exist or if no value is defined for this parameter.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @param defaultValue Default value.
         * @see #setAdvancedParam(const std::string&, double)
         */
        int getAdvancedParam(const std::string& key, double defaultValue) const {
            return ls_get_advanced_double_param(bsolver, key.c_str(), defaultValue);
        }
        
        /**
         * Returns the value of an advanced parameter. Advanced parameters are reserved for 
         * fine tuning or debugging and should not be used by end-users. The default value is 
         * returned if the parameter does not exist or if no value is defined for this parameter.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param key Name of the parameter.
         * @param defaultValue Default value of the parameter.
         * @see #setAdvancedParam(const std::string&, const std::string&)
         */
        std::string getAdvancedParam(const std::string& key, const std::string& defaultValue) const {
            int size = ls_get_advanced_str_param(bsolver, key.c_str(), defaultValue.c_str(), NULL, 0);
            std::string str(size, '\0');
            ls_get_advanced_str_param(bsolver, key.c_str(), defaultValue.c_str(), &str[0], size);
            return str;
        }
        
        /** 
         * Returns a string representation of these parameters. 
         * Useful for debugging or logging purposes. 
         * 
         * @return String representation. 
         */
        std::string toString() const {
            int size = ls_param_to_string(bsolver, NULL, 0);
            std::string str(size, '\0');
            ls_param_to_string(bsolver, &str[0], size);
            return str;
        }

        ~LSParam() {}
        LSParam(const LSParam& other) { bsolver = other.bsolver; }
        LSParam& operator=(const LSParam& other) { bsolver = other.bsolver; return *this; }

    private:
        friend class LocalSolver;

        lssolver* bsolver;
        LSParam(lssolver* solver) : bsolver(solver) {}
    };
}

#endif
