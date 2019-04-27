// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXSOLVER_H
#define LS_BLACKBOXSOLVER_H

#include "lsbberrorcode.h"
#include "lsbbexception.h"
#include "lsbbexpression.h"
#include "lsbbmodel.h"
#include "lsbbnativecontext.h"
#include "lsbbnativefunction.h"
#include "lsbbobjectivedirection.h"
#include "lsbboperator.h"
#include "lsbbparam.h"
#include "lsbbsolution.h"
#include "lsbbstate.h"
#include "lsbbsymbols.h"
#include "lsbbvalue.h"

/**
 * Namespace of LocalSolverBlackBox
 */   
namespace localsolverblackbox{
    /**
     * LocalSolverBlackBox environment. Main class of LocalSolverBlackBox library. Here are described the 
     * basic steps for using LocalSolverBlackBox:
     * -# Build your model ({@link LSBBModel}) by creating some expressions ({@link LSBBExpression}).
     * -# If desired, parameterize the solver ({@link LSBBParam}).
     * -# Run the solver ({@link LocalSolverBlackBox}).
     * -# Retrieve the best solution found by the solver ({@link LSBBSolution}).
     *
     * <b>Note that this API is not thread safe.</b> If multiple threads access and modify 
     * a same LocalSolverBlackBox environment, it must be synchronized. The only methods that can 
     * be used without synchronization are {@link #getState()} and {@link #stop()}.
     */
    class LocalSolverBlackBox {
    public:
        
        /** 
         * Constructs a complete LocalSolverBlackBox environment and take a token license.
         * If no token is available or if the token server is not accessible, 
         * an exception is thrown. The token license is released when the destructor 
         * of this LocalSolverBlackBox environment is called.
         */        
        LocalSolverBlackBox() {
            lsbb_set_exception_callback(LocalSolverBlackBox::errorThrown, NULL);
            bbsolver = lsbb_create_solver();
        }

        /** 
         * Deletes this LocalSolverBlackBox environment and all associated objects.
         * In particular, the model, its expressions, the phases, the statistics, 
         * and the solution are deleted. The token license is then released.
         */
        ~LocalSolverBlackBox() {
            lsbb_delete_solver(bbsolver);
            bbsolver = NULL;
        }

        /** 
         * Gets the state of this LocalSolverBlackBox environment. This method can be called 
         * in any state. In particular, this method can be called in state {@link #S_Running}.
         * 
         * @return State of LocalSolverBlackBox.
         */
        LSBBState getState() const {
            return lsbb_get_state(bbsolver);
        }

        /** 
         * Gets the model associated to this LocalSolverBlackBox environment. Once the model 
         * is created and closed, the solver can be launched with {@link #solve()}.  
         * 
         * @return Model.
         */
        LSBBModel getModel() const {
            return LSBBModel(bbsolver);
        }
        
        /** 
         * Gets the parameters of this LocalSolverBlackBox environment. 
         * 
         * @return Parameters.
         */
        LSBBParam getParam() const {
            return LSBBParam(bbsolver);
        }

        /**
         * Solves the model. This method returns only when the time limit or
         * the evaluation limit are reached.
         * The model must be closed to launch the resolution.
         */
        void solve() {
            lsbb_solve(bbsolver);
        }
        
        /**
         * Aborts the resolution previously launched using {@link #solve()}.
         * If no resolution was launched, this method does nothing.
         * Called from another thread, this method enables users
         * to stop the resolution properly. Solution 
         * remains valid. This method can be called in any state, 
         * notably in state {@link #S_Running}.
         */
        void stop(){
            lsbb_stop(bbsolver);
        }
        
        /**
         * Gets the best solution found by the solver. If the solver has not been started at least once, 
         * all the decision variables of the solution are set to 0 (such a solution may be infeasible).
         * Only allowed in states {@link #S_Paused} or {@link #S_Stopped}.
         * 
         * @return Best solution.
         */
        LSBBSolution getSolution() const{
            return LSBBSolution(bbsolver);
        }
        
        
    private:
        lsbbsolver* bbsolver;
        
        static void errorThrown(LSBBErrorCode code, const char* message, const char* filename, const char* funcname, int lineno, void*) {
            throw LSBBException(code, filename, lineno, funcname, message);
        }
    };
}

#endif
