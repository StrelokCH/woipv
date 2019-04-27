// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXEVALUATIONPOINT_H
#define LS_BLACKBOXEVALUATIONPOINT_H

#include "bbentrypoint.h"

namespace localsolverblackbox {
    /**
     * An evaluation point associated with a native function. Evaluation points are used to specify known points
     * for a function in advance. It can be useful to warm-start the solver when the function is particularly
     * expensive to evaluate, or if you already have a good estimate of the optimal point.
     * 
     * This information may be used by the solver to drive the solution process. The exact behaviour is
     * implementation-defined and the solver may perform the exact same call a second time or ignore
     * the values altogether.
     * 
     * @see LSBBModel
     */
    class LSBBEvaluationPoint {
    public:
        /** 
         * Adds an integer argument with the given value.
         * Only allowed in state {@link #S_Stopped}.
         *  
         * @param value Value of the argument.
         * @see LSBBEvaluationPoint#getArgumentInt()
         */
        void addArgumentInt(lsint value) {
            lsbb_evaluation_point_add_argument_int(bsolver, pointId, value);
        }
        /** 
         * Adds a double argument with the given value.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param value Value of the argument.
         * @see LSBBEvaluationPoint#getArgumentDouble()
         */
        void addArgumentDouble(lsdouble value) {
            lsbb_evaluation_point_add_argument_double(bsolver, pointId, value);
        }
        /** 
         * Sets the returned value for this point.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @see LSBBEvaluationPoint#getReturnedValue()
         * @param value Returned value at this point.
         */
        void setReturnedValue(lsdouble value) {
            lsbb_evaluation_point_set_returned_value(bsolver, pointId, value);
        }
        
        /**
         * Returns the value of the argument at the given position. 
         * Only allowed in state {@link #S_Stopped}.
         * Only allowed if this argument is an integer.
         * 
         * @param pos Position in the argument list.
         * @see LSBBEvaluationPoint#addArgumentInt()
         * @see LSBBEvaluationPoint#isArgumentInt()
         * @see LSBBEvaluationPoint#setArgumentInt()
         * @return Value of the argument.
         */
        lsint getArgumentInt(unsigned int pos) {
            return lsbb_evaluation_point_get_argument_int(bsolver, pointId, pos);
        }
        /**
         * Returns the value of the argument at the given position.
         * Only allowed in state {@link #S_Stopped}.
         * Only allowed if this argument is a double.
         * 
         * @param pos Position in the argument list.
         * @see LSBBEvaluationPoint#addArgumentDouble()
         * @see LSBBEvaluationPoint#isArgumentDouble()
         * @see LSBBEvaluationPoint#setArgumentDouble()
         * @return Value of the argument.
         */
        lsdouble getArgumentDouble(unsigned int pos) {
            return lsbb_evaluation_point_get_argument_double(bsolver, pointId, pos);
        }
        
        /** 
         * Returns true if the argument at the given position is an integer, false otherwise.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param pos Position in the argument list. 
         */
        bool isArgumentInt(unsigned int pos) {
            return lsbb_evaluation_point_is_argument_int(bsolver, pointId, pos);
        }
        /** 
         * Returns true if the argument at the given position is a double, false otherwise.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param pos Position in the argument list. 
         */
        bool isArgumentDouble(unsigned int pos) {
            return lsbb_evaluation_point_is_argument_double(bsolver, pointId, pos);
        }
        
        /** 
         * Sets the value of the argument at the given position.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param pos Position in the argument list.
         * @param value Value of the argument.
         * @see LSBBEvaluationPoint#getArgumentInt()
         */
        void setArgumentInt(unsigned int pos, lsint value) {
            lsbb_evaluation_point_set_argument_int(bsolver, pointId, pos, value);
        }
        /** 
         * Sets the value of the argument at the given position.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param pos Position in the argument list.
         * @param value Value of the argument.
         * @see LSBBEvaluationPoint#getArgumentDouble()
         */
        void setArgumentDouble(unsigned int pos, lsdouble value) {
            lsbb_evaluation_point_set_argument_double(bsolver, pointId, pos, value);
        }
        
        /** 
         * Gets the returned value for this point. If the value was not set by the
         * user, this defaults to NaN. Only allowed in state {@link #S_Stopped}.
         * 
         * @see LSBBEvaluationPoint#setReturnedValue()
         * @return Returned value for this evaluation point.
         */
        lsdouble getReturnedValue() {
            return lsbb_evaluation_point_get_returned_value(bsolver, pointId, 0);
        }
        
    private:
        friend class LSBBModel;
        
        LSBBEvaluationPoint(BBSolver* solver, int pointId) : bsolver(solver), pointId(pointId) {
        }
        
        BBSolver* bsolver;
        unsigned int pointId;
    };
}


#endif

