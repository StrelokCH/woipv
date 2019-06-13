// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXENTRYPOINT_H
#define LS_BLACKBOXENTRYPOINT_H

#include "lsbbsymbols.h"
#include "lsbbvalue.h"
#include "lsbbstate.h"
#include "lsbboperator.h"
#include "lsbbobjectivedirection.h"
#include "lsbberrorcode.h"

namespace localsolverblackbox{
    
    class BBSolver;
    class BBExternalBuffer;

    typedef BBSolver lsbbsolver; 
    typedef BBExternalBuffer lsbbbuffer;
    typedef LSBBState lsbbstate;
    typedef LSBBObjectiveDirection lsbbobjdirection;
    typedef LSBBErrorCode lsbberrorcode;
    typedef LSBBOperator lsbboperator;
    
    typedef void (*lsbbexceptioncallback)(lsbberrorcode code, const char* message, const char* filename, const char* funcname, int lineno, void* userdata);
    typedef lsdouble (*lsbbnativefunction) (lsbbsolver* solver, lsbbbuffer* buffer, void* userdata);

    LIB_PUBLIC int lsbb_buffer_size(lsbbsolver* solver, lsbbbuffer* buffer); 
    LIB_PUBLIC bool lsbb_buffer_is_bool(lsbbsolver* solver, lsbbbuffer* buffer, int pos);
    LIB_PUBLIC bool lsbb_buffer_is_int(lsbbsolver* solver, lsbbbuffer* buffer, int pos);
    LIB_PUBLIC bool lsbb_buffer_is_double(lsbbsolver* solver, lsbbbuffer* buffer, int pos);
    
    LIB_PUBLIC lsint lsbb_buffer_get_int_value(lsbbsolver* solver, lsbbbuffer* buffer, int pos);
    LIB_PUBLIC lsdouble lsbb_buffer_get_double_value(lsbbsolver* solver, lsbbbuffer* buffer, int pos);
    
    LIB_PUBLIC bool lsbb_evaluation_point_is_argument_int(lsbbsolver* solver, int pointId, int pos);
    LIB_PUBLIC bool lsbb_evaluation_point_is_argument_double(lsbbsolver* solver, int pointId, int pos);
    
    LIB_PUBLIC lsint lsbb_evaluation_point_get_argument_int(lsbbsolver* solver, int pointId, int pos);
    LIB_PUBLIC lsdouble lsbb_evaluation_point_get_argument_double(lsbbsolver* solver, int pointId, int pos);
    LIB_PUBLIC lsdouble lsbb_evaluation_point_get_returned_value(lsbbsolver* solver, int pointId, int pos);
    
    LIB_PUBLIC void lsbb_evaluation_point_add_argument_int(lsbbsolver* solver, int pointId, lsint value);
    LIB_PUBLIC void lsbb_evaluation_point_add_argument_double(lsbbsolver* solver, int pointId, lsdouble value);
    
    LIB_PUBLIC void lsbb_evaluation_point_set_argument_int(lsbbsolver* solver, int pointId, int pos, lsint value);
    LIB_PUBLIC void lsbb_evaluation_point_set_argument_double(lsbbsolver* solver, int pointId, int pos, lsdouble value);
    LIB_PUBLIC void lsbb_evaluation_point_set_returned_value(lsbbsolver* solver, int pointId, lsdouble value);
    
    LIB_PUBLIC lsbbsolver* lsbb_create_solver();
    LIB_PUBLIC void lsbb_delete_solver(lsbbsolver* solver);
    
    LIB_PUBLIC lsbbstate lsbb_get_state(lsbbsolver* solver);
    
    LIB_PUBLIC void lsbb_solve(lsbbsolver* solver);
    LIB_PUBLIC void lsbb_stop(lsbbsolver* solver);
    
    LIB_PUBLIC int lsbb_create_int_constant(lsbbsolver* solver, lsint value);
    LIB_PUBLIC int lsbb_create_double_constant(lsbbsolver* solver, lsdouble value);
    LIB_PUBLIC int lsbb_create_expression(lsbbsolver* solver, lsbboperator op);
    
    LIB_PUBLIC int  lsbb_create_native_function(lsbbsolver* solver, lsbbnativefunction func, void* userdata);
    LIB_PUBLIC int lsbb_create_evaluation_point(lsbbsolver* solver, int exprId);
    
    LIB_PUBLIC void lsbb_add_objective(lsbbsolver* solver, int exprId, lsbbobjdirection direction);
    LIB_PUBLIC void lsbb_close(lsbbsolver* solver);
    LIB_PUBLIC bool lsbb_is_closed(lsbbsolver* solver);
    
    LIB_PUBLIC void lsbb_expr_add_operand(lsbbsolver* solver, int exprId, int operandId);
    
    LIB_PUBLIC void lsbb_set_seed(lsbbsolver* solver, int seed);
    LIB_PUBLIC int lsbb_get_seed(lsbbsolver* solver);
    
    LIB_PUBLIC void lsbb_set_time_limit(lsbbsolver* solver, int limit);
    LIB_PUBLIC int lsbb_get_time_limit(lsbbsolver* solver);
    
    LIB_PUBLIC void lsbb_set_evaluation_limit(lsbbsolver* solver, int limit);
    LIB_PUBLIC int lsbb_get_evaluation_limit(lsbbsolver* solver);
    
    LIB_PUBLIC lsint lsbb_expr_get_int_value(lsbbsolver* solver, int exprId);
    LIB_PUBLIC lsdouble lsbb_expr_get_double_value(lsbbsolver* solver, int exprId);
    
    LIB_PUBLIC void lsbb_set_exception_callback(lsbbexceptioncallback callback, void* userdata);
    
    LIB_PUBLIC int lsbb_major_version();
    LIB_PUBLIC int lsbb_minor_version();
    LIB_PUBLIC int lsbb_version(char* buffer, int bufferSize);
    LIB_PUBLIC void lsbb_interrupt(lsbbsolver* solver, const char* reason);
    
    LIB_PUBLIC bool lsbb_expr_is_bool(lsbbsolver* solver, int exprId);
    LIB_PUBLIC bool lsbb_expr_is_int(lsbbsolver* solver, int exprId);
    LIB_PUBLIC bool lsbb_expr_is_double(lsbbsolver* solver, int exprId);
    
    LIB_PUBLIC bool lsbb_check_paused_or_stopped(lsbbsolver* solver);
    LIB_PUBLIC bool lsbb_check_not_running(lsbbsolver* solver);
    LIB_PUBLIC bool lsbb_expr_check_index(lsbbsolver* solver, int exprIndex);
    
    LIB_PUBLIC void lsbb_set_objective_bound(lsbbsolver* solver, lsdouble bound);
}

#endif
