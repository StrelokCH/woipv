// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LOCALSOLVER_H
#define LS_LOCALSOLVER_H

#include "lsvalue.h"
#include "lsexception.h"
#include "lsexpression.h"
#include "lsmodel.h"
#include "lsobjectivedirection.h"
#include "lsoperator.h"
#include "lsparam.h"
#include "lsphase.h"
#include "lssolution.h"
#include "lssolutionstatus.h"
#include "lsstate.h"
#include "lsstatistics.h"
#include "lsversion.h"
#include "lserrorcode.h"
#include "lscallbacktype.h"
#include "lscallback.h"
#include "lsnativefunction.h"
#include "lsnativecontext.h"
#include "lsinconsistency.h"
#include "lscollection.h"
#include "lsarray.h"
#include "entrypoint.h"
#include <string>

/**
 * Namespace of %LocalSolver.
 */
namespace localsolver {

    /**
     * LocalSolver environment. Main class of %LocalSolver library. Here are described the 
     * basic steps for using %LocalSolver:
     * -# Build your model ({@link LSModel}) by creating some expressions ({@link LSExpression}).
     * -# If desired, parameterize and add phases to the solver ({@link LSParam}, {@link LSPhase}).
     * -# Run the solver ({@link LocalSolver}).
     * -# Retrieve the best solution found by the solver ({@link LSSolution}).
     * -# Consult the statistics of the resolution ({@link LSStatistics}).
     *
     * <b>Note that this API is not thread safe.</b> If multiple threads access and modify 
     * a same LocalSolver environment, it must be synchronized. The only methods that can 
     * be used without synchronization are {@link #getState()} and {@link #stop()}.
     * 
     * Please consult {@link LSVersion} for copyright and version info.
     */
    class LocalSolver {
    public:

        /** 
         * Constructs a complete LocalSolver environment and take a token license.
         * If no token is available or if the token server is not accessible, 
         * an exception is thrown. The token license is released when the destructor 
         * of this LocalSolver environment is called.
         */
        LocalSolver() {
            ls_set_exception_callback(LocalSolver::errorThrown, NULL);
            bsolver = ls_create_solver();
        }

        /** 
         * Deletes this LocalSolver environment and all associated objects.
         * In particular, the model, its expressions, the phases, the statistics, 
         * and the solution are deleted. The token license is then released.
         */
        ~LocalSolver() {
            ls_delete_solver(bsolver);
            bsolver = NULL;
            for (std::vector<LSModel::DisposableHolder*>::iterator it = disposables.begin();
                    it != disposables.end(); ++it) {
                delete *it;
            }
        }

        /** 
         * Gets the state of this LocalSolver environment. This method can be called 
         * in any state. In particular, this method can be called in state {@link #S_Running}.
         * 
         * @return State of LocalSolver.
         */
        LSState getState() const {
            return ls_get_state(bsolver);
        }

        /** 
         * Gets the model associated to this LocalSolver environment. Once the model 
         * is created and closed, the solver can be launched with {@link #solve()}.  
         * 
         * @return Model.
         */
        LSModel getModel() const {
            return LSModel(bsolver, &disposables);
        }

        /** 
         * Gets the parameters of this LocalSolver environment. 
         * 
         * @return Parameters.
         */
        LSParam getParam() const {
            return LSParam(bsolver);
        }

        /**
         * Solves the model. This method returns only when the time limit,
         * the iteration limit or the objective bounds are reached.
         * The model must be closed to launch the resolution.
         */
        void solve() {
            ls_solve(bsolver);
        }

        /**
         * Compute an inconsistency core for this model.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @return The inconsistency core
         */
        LSInconsistency computeInconsistency() const {
            ls_compute_inconsistency(bsolver);
            return LSInconsistency(bsolver);
        }
        
        /**
         * Aborts the resolution previously launched using {@link #solve()}.
         * If no resolution was launched, this method does nothing.
         * Called from another thread, this method enables users
         * to stop the resolution properly. Solution and statistics 
         * remain valid. This method can be called in any state, 
         * notably in state {@link #S_Running}.
         */
        void stop() {
            ls_stop(bsolver);
        }

        /** 
         * Adds a new phase to this LocalSolver environment.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @return Created phase.
         */
        LSPhase createPhase() {
            return LSPhase(bsolver, ls_add_phase(bsolver));
        }

        /**
         * Gets the phase with the given index.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @param phaseIndex Index of the phase.
         * @return Phase.
         */
        LSPhase getPhase(int phaseIndex) const {
            return LSPhase(bsolver, phaseIndex);
        }

        /**
         * Gets the number of phases.
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Number of phases.
         */
        int getNbPhases() const {
            return ls_nb_phases(bsolver);
        }

        /**
         * Gets the best solution found by the solver. If the solver has not been
         * started at least once, all the decision variables of the solution are
         * set to 0, or to their closest bound if 0 is not part of their domain
         * (such a solution may be infeasible).
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * 
         * @return Best solution.
         */
        LSSolution getSolution() const {
            return LSSolution(bsolver);
        }

        /** 
         * Gets the statistics of this LocalSolver environment. Statistics are reset to zero 
         * before each resolution. Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * Note that for performance reasons, this function always returns the same object.
         * 
         * @return Statistics.
         * @see LSStatistics
         */
        LSStatistics getStatistics() const {
            return LSStatistics(bsolver, -1);
        }

        /** 
         * Returns a string representation of this LocalSolver environment. This representation provides useful info 
         * related to the model, the parameters, and the phases (if any). Useful for debugging or logging purposes. 
         * 
         * @return String representation of this LocalSolver. 
         */
        std::string toString() const {
            int size = ls_solver_to_string(bsolver, NULL, 0);
            std::string str(size, '\0');
            ls_solver_to_string(bsolver, &str[0], size);
            return str;
        }

        /**
         * Returns useful info about the search while running. Only allowed if the solver has been started at least once. 
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}. Useful for debugging or logging purposes.
         * The displayed information depends on the selected verbosity.isDescent
         * Here are some explanations about the output string for the highest verbosity (2):
         * - "sec" stands for the number of seconds.
         * - "itr" stands for the number of iterations.
         * - "infeas" corresponds to the infeasibility score of the best solution found, if infeasible.
         * - "obj" corresponds to the objective values of the best solution found, if feasible.
         * - "mov" corresponds to the number of moves performed.
         * - "inf" corresponds to the percentage of infeasible moves.
         * - "acc" corresponds to the percentage of accepted moves.
         * - "imp" corresponds to the number of improving moves. 
         * 
         * @return Info about the search while running.
         */
        std::string getInfo() const {
            int size = ls_solver_info(bsolver, NULL, 0);
            std::string str(size, '\0');
            ls_solver_info(bsolver, &str[0], size);
            return str;
        }

        /** 
         * Export a complete environment or a model to a file.
         * 
         * Currently, this function supports 2 file formats :
         * 
         * - LSB : with this format, the complete environment (model, parameters, solution, ...) 
         *   is exported. This format is useful to debug or replay a model in the same conditions.
         *   Since the produced file is binary, it offers good performance and space efficiency.
         * - LSP : with this format, only the model is exported in the LSP language.
         *   This format is useful to have a quick view of the optimization model or to perform some
         *   modifications. However, the file may be heavy for big models and the exact 
         *   reproductibility is not guaranteed since the parameters from one execution to another
         *   may differ.
         * 
         * The chosen file format is determined by the file suffix. An exception
         * is thrown if the provided file suffix is not supported. The suffix
         * may optionally be followed by .gz. In that case, this function
         * produces a compressed result (using deflate algorithm).
         * 
         * @param filename Name of the file. 
         * @see LocalSolver#loadEnvironment
         * @since 3.0
         */
        void saveEnvironment(const std::string& filename) const {
            ls_save_environment(bsolver, filename.c_str());
        }

        /** 
         * Import a complete environment from a file. Only allowed in
         * state {@link #S_Modeling}. The current model must be empty.
         * 
         * The only format supported is LSB.
         * 
         * The chosen file format is determined by the file suffix. An exception
         * is thrown if the provided file suffix is not supported. The suffix
         * may optionally be followed by .gz. In that case, this function
         * uncompress the stream before reading.
         * 
         * @param filename Name of the file. 
         * @see LocalSolver#saveEnvironment
         * @since 3.0
         */
        void loadEnvironment(const std::string& filename) {
            ls_load_environment(bsolver, filename.c_str());
        }
        
        /**
         * Add a new callback for a specific event type.
         * The callback will be called each time the given event occurs.
         * 
         * The same callback can be used for different events.
         * Only allowed in states {@link #S_Stopped} or {@link #S_Modeling}.
         * 
         * <strong>Note 1</strong>: When a callback is called, the solver is paused.
         * In that state, you can call all the methods marked as
         * "allowed in state {@link #S_Paused}". Calling any other method will
         * throw an error. For example, you can stop the resolution
         * from a callback, retrieve the current solution or retrieve
         * the statistics of the solver but you can't remove a constraint.
         * 
         * <strong>Note 2</strong>: Please note that %LocalSolver does not manage
         * memory of objects created outside of its environment. Thus, you have
         * to explicitly remove and delete your LSCallback at the end of the search.
         * 
         * @param type Event to watch.
         * @param callback Callback. Cannot be null.
         * @see LSCallback
         * @since 4.0
         */
        void addCallback(LSCallbackType type, LSCallback* callback) {
            ls_add_callback(bsolver, type, &LocalSolver::callbackDispatch, (void*)callback);
        }
        
        /**
         * Remove the callback for the given event type.
         * 
         * @param type Event.
         * @param callback User callback to delete.
         * @return false if the callback was not added for the given event, true otherwise.
         * @see LocalSolver#addCallback
         * @see LSCallback
         * @since 4.0
         */
        bool removeCallback(LSCallbackType type, LSCallback* callback) {
            return ls_remove_callback_2(bsolver, type, &LocalSolver::callbackDispatch, (void*)callback);
        }
        
    private:

        friend class LSModel;
        friend class LSExpression;
        friend class LSParam;
        friend class LSSolution;
        friend class LSStatistics;
        friend class Launcher;

        lssolver* bsolver;
        mutable std::vector<LSModel::DisposableHolder*> disposables;
        
        LocalSolver(const LocalSolver&) { }
        LocalSolver(lssolver* bsolver) : bsolver(bsolver) {}
        LocalSolver& operator=(const LocalSolver& other) { bsolver = other.bsolver; return *this; }
        void detach() { bsolver = NULL; }
        
        struct DetachGuard {
            LocalSolver& ls;
            DetachGuard(LocalSolver& ls) : ls(ls) {}
            ~DetachGuard() { ls.detach(); }
        };

        static void errorThrown(LSErrorCode code, const char* message, const char* filename, const char* funcname, int lineno, void*) {
            throw LSException(code, filename, lineno, funcname, message);
        }
        
        static void callbackDispatch(lssolver* solver, lscallbacktype type, void* userdata) {
            LocalSolver ls(solver);
            DetachGuard guard(ls);
            ((LSCallback*)userdata)->callback(ls, type);
        }
    };
}

#endif
