// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXMODEL_H
#define LS_BLACKBOXMODEL_H

#include "lsbbvalue.h"
#include "lsbbexpression.h"
#include "lsbbevaluationpoint.h"
#include "lsbbnativefunction.h"
#include "lsbbnativecontext.h"
#include "lsbbobjectivedirection.h"
#include "lsbboperator.h"
#include "bbentrypoint.h"

#include <limits>

namespace localsolverblackbox{
    
    /**
     * Mathematical optimization model.
     * A model is composed of a native function (the blackbox), a number of decisions 
     * and a function call that associates the decisions and the function. 
     * Once your optimization model is created and closed, the solver can be 
     * launched to resolve it. Note that you cannot modify a model which has been 
     * closed. 
     *
     * @see LSBBExpression
     * @see LSBBOperator
     */
    class LSBBModel{
    public:
        
        /**
         * Creates a call expression. 
         */
        LSBBExpression call(){
            return LSBBExpression(bsolver, lsbb_create_expression(bsolver, O_Call));
        }
        
        /**
         * Creates a native function. Once you have instantiated it,
         * you have to pass arguments to your function and call it. For that, you
         * have to create expressions of type {@link #O_Call}. The first operand must
         * be your native function. The other operands must be LSBBExpressions.
         * Their value will be made accessible to your native function through
         * the native context.
         * 
         * <strong>Note 1:</strong> Most of the time your native function will be called
         * when the solver is in state {@link #S_Running}. 
         * Do not attempt to call any method of the solver (to retrieve statistics,
         * values of LSBBExpressions or whatever) in that state or an exception will
         * be thrown. The only accessible function is {@link LocalSolverBlackBox#stop()}.
         *
         * <strong>Note 2:</strong> LocalSolverBlackBox does not manage memory of objects
         * created outside of its environment. Thus, you have to explicitly
         * delete your LSBBNativeFunction at the end of the search.
         * 
         * @param func Native function to call.
         * @return The expression associated to the function.
         * @see #O_NativeFunction
         */
        LSBBExpression createNativeFunction(LSBBNativeFunction* func){
            return LSBBExpression(bsolver, lsbb_create_native_function(bsolver, &LSBBModel::nativeFuncDispatch, (void*)func));
        }
        
        /**
         * Creates an evaluation point. Once it is instantiated,
         * you have to set the values for the function's arguments
         * and the value returned by the function at this point.
         * Only allowed in state {@link #S_Stopped}.
         * 
         * @param expr Expression of type #O_NativeFunction.
         * @return An evaluation point associated with the function.
         * @see #O_NativeFunction
         * @see LSBBEvaluationPoint#addArgumentDouble()
         * @see LSBBEvaluationPoint#addArgumentInt()
         * @see LSBBEvaluationPoint#setReturnedValue()
         */
        LSBBEvaluationPoint createEvaluationPoint(LSBBExpression expr) {
            return LSBBEvaluationPoint(bsolver, lsbb_create_evaluation_point(bsolver, expr.exprId));
        }

        /**
         * Creates a boolean decision. Binary decision variable with domain [0.1].
         * @see #O_Bool
         */
        LSBBExpression boolVar(){
            return LSBBExpression(bsolver, lsbb_create_expression(bsolver, O_Bool));
        }
        
        /**
         * Creates an integer decision. Decision variable with domain [min,max]. 
         * @see #O_Int
         */
        LSBBExpression intVar(lsint minValue, lsint maxValue){
            LSBBExpression expr(bsolver, lsbb_create_expression(bsolver,O_Int));
            LSBBExpression minValueExpr(bsolver, lsbb_create_int_constant(bsolver, minValue));
            LSBBExpression maxValueExpr(bsolver, lsbb_create_int_constant(bsolver, maxValue));
            expr.addOperand(minValueExpr);
            expr.addOperand(maxValueExpr);
            return expr;
        }
        
        /**
         * Creates a float decision. Decision variable with domain [min,max]. 
         * @see #O_Float
         */
        LSBBExpression floatVar(lsdouble minValue, lsdouble maxValue){
            LSBBExpression expr(bsolver, lsbb_create_expression(bsolver,O_Float));
            LSBBExpression minValueExpr(bsolver, lsbb_create_double_constant(bsolver, minValue));
            LSBBExpression maxValueExpr(bsolver, lsbb_create_double_constant(bsolver, maxValue));
            expr.addOperand(minValueExpr);
            expr.addOperand(maxValueExpr);
            return expr;
        }
        
        /** 
         * Sets the given expression to be the current objective to optimize. Only allowed in state {@link #S_Modeling}. 
         *
         * @param expr Expression of type #O_Call.
         * @param dir Optimization direction of this objective.
         */
        void addObjective(LSBBExpression expr, LSBBObjectiveDirection dir){
            lsbb_add_objective(bsolver, expr.exprId, dir);
        }
        
        /** 
         * Closes the model. Only allowed in state {@link #S_Modeling}.
         * Once the model is closed, no expression, constraints or objectives
         * can be added. The model must be closed before starting its resolution.
         */
        void close(){
            lsbb_close(bsolver);
        }
        
        /** 
         * Returns true if the model is closed, false otherwise. 
         * 
         * @return True if the model is closed.
         */
        bool isClosed() const {
            return lsbb_is_closed(bsolver);
        }
        
        ~LSBBModel() {
        }

        LSBBModel() {
            bsolver = NULL;
        }

        LSBBModel(const LSBBModel& other) {
            bsolver = other.bsolver;
        }

        LSBBModel& operator=(const LSBBModel& other) {
            bsolver = other.bsolver;
            return *this;
        }
        
    private:
        friend class LocalSolverBlackBox;
        
        explicit LSBBModel(lsbbsolver* solver) : bsolver(solver) {
        }

        static lsdouble nativeFuncDispatch(lsbbsolver* solver, lsbbbuffer* buffer, void* userdata) {
            LSBBNativeContext context(solver, buffer);
            lsdouble result = std::numeric_limits<lsdouble>::quiet_NaN();;
            try{
                result = ((LSBBNativeFunction*)userdata)->call(context);
            }
            catch(std::exception e){
                lsbb_interrupt(solver, e.what());
            }
            return result;
        }
        
        lsbbsolver* bsolver;
    };
}

#endif
