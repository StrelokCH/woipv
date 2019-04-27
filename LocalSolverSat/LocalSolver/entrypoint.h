// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_ENTRYPOINT_H
#define LS_ENTRYPOINT_H

#include "lssolutionstatus.h"
#include "lsobjectivedirection.h"
#include "lscallbacktype.h"
#include "lserrorcode.h"
#include "lscallback.h"
#include "lsstate.h"
#include "lsoperator.h"
#include "lsvalue.h"
#include "lssymbols.h"
#include <iterator>

#define LS_CHECK_DEFINED(ptr) { \
    if(ptr == NULL) \
        throw LSException(EC_Api, __FILE__, __LINE__, __FUNCTION__, "The object is not defined or does not belong to the solver"); \
}

#define LS_CHECK_SAME_MODEL(sol1, sol2) { \
    LS_CHECK_DEFINED(sol1); \
    LS_CHECK_DEFINED(sol2); \
    if((sol1) != (sol2)) \
        throw LSException(EC_Api, __FILE__, __LINE__, __FUNCTION__, "The objects don't belong to the same model."); \
}

#if !defined(LS_CPP11) && (__cplusplus >= 201103l || (defined(_MSC_VER) && _MSC_VER >= 1800))
#define LS_CPP11
#endif

namespace localsolver {
    struct with_iterator {};
    struct without_iterator {};
    template<typename T>
    with_iterator is_iterator(typename T::iterator_category *) { return with_iterator(); }
    template<typename T>
    without_iterator is_iterator(...) { return without_iterator(); }

    class BSolver;
    class BArray;
    class BBuffer;
    class LSExpression;

    template<typename T>
    struct LSCompatibleArg;
    template<>
    struct LSCompatibleArg<lsint> { typedef LSExpression type; };
    template<>
    struct LSCompatibleArg<int> { typedef LSExpression type; };
    template<>
    struct LSCompatibleArg<lsdouble> { typedef LSExpression type; };
    
    typedef BSolver lssolver;
    typedef BArray lsarray;
    typedef LSSolutionStatus lssolutionstatus;
    typedef LSState lsstate;
    typedef LSOperator lsoperator;
    typedef LSObjectiveDirection lsobjdirection;
    typedef LSCallbackType lscallbacktype;
    typedef LSErrorCode lserrorcode;
    typedef BBuffer lsbuffer;
    
    typedef void (*lsexceptioncallback)(lserrorcode code, const char* message, const char* filename, const char* funcname, int lineno, void* userdata);
    typedef lsdouble (*lsnativefunction) (lssolver* solver, lsbuffer* buffer, void* userdata);
    typedef void (*lscallback)(lssolver* solver, lscallbacktype type, void* userdata);
    
    LIB_PUBLIC lssolver* ls_create_solver();
    LIB_PUBLIC void ls_delete_solver(lssolver* solver);
    LIB_PUBLIC lsstate ls_get_state(lssolver* solver);
    LIB_PUBLIC void ls_solve(lssolver* solver);
    LIB_PUBLIC void ls_compute_inconsistency(lssolver* solver);    
    LIB_PUBLIC void ls_stop(lssolver* solver);
    LIB_PUBLIC void ls_interrupt(lssolver* solver, const char* reason);
    LIB_PUBLIC int ls_add_phase(lssolver* solver);
    LIB_PUBLIC int ls_nb_phases(lssolver* solver);
    LIB_PUBLIC int ls_solver_to_string(lssolver* solver, char* buffer, int bufferSize);
    LIB_PUBLIC int ls_param_to_string(lssolver* solver, char* buffer, int bufferSize);
    LIB_PUBLIC int ls_solver_info(lssolver* solver, char* buffer, int bufferSize);
    LIB_PUBLIC bool ls_check_paused_or_stopped(lssolver* solver);
    LIB_PUBLIC bool ls_check_not_running(lssolver* solver);

    LIB_PUBLIC void ls_save_environment(lssolver* solver, const char* filename);
    LIB_PUBLIC void ls_load_environment(lssolver* solver, const char* filename);
    LIB_PUBLIC void ls_add_callback(lssolver* solver, lscallbacktype type, lscallback callback, void* userdata);
    LIB_PUBLIC bool ls_remove_callback(lssolver* solver, lscallbacktype type, lscallback callback);
    LIB_PUBLIC bool ls_remove_callback_2(lssolver* solver, lscallbacktype type, lscallback callback, void* userdata);
    
    LIB_PUBLIC void ls_set_seed(lssolver* solver, int seed);
    LIB_PUBLIC int ls_get_seed(lssolver* solver);
    LIB_PUBLIC void ls_set_nb_threads(lssolver* solver, int nbThreads);
    LIB_PUBLIC int ls_get_nb_threads(lssolver* solver);
    LIB_PUBLIC void ls_set_annealing_level(lssolver* solver, int annealingLevel);
    LIB_PUBLIC int ls_get_annealing_level(lssolver* solver);
    LIB_PUBLIC void ls_set_verbosity(lssolver* solver, int verbosity);
    LIB_PUBLIC int ls_get_verbosity(lssolver* solver);
    LIB_PUBLIC void ls_set_time_between_displays(lssolver* solver, int timeBetweenDisplays);
    LIB_PUBLIC int ls_get_time_between_displays(lssolver* solver);
    LIB_PUBLIC void ls_set_time_between_ticks(lssolver* solver, int timeBetweenTicks);
    LIB_PUBLIC int ls_get_time_between_ticks(lssolver* solver);
    LIB_PUBLIC void ls_set_iteration_between_ticks(lssolver* solver, long long iterationBetweenTicks);
    LIB_PUBLIC long long ls_get_iteration_between_ticks(lssolver* solver);
    
    LIB_PUBLIC void ls_set_log_file(lssolver* solver, const char* path);
    LIB_PUBLIC int ls_get_log_file(lssolver* solver, char* buffer, int bufferSize);
    LIB_PUBLIC bool ls_advanced_param_is_set(lssolver* solver, const char* param);
    LIB_PUBLIC int ls_get_advanced_param(lssolver* solver, const char* param, int defaultValue);
    LIB_PUBLIC void ls_set_advanced_param(lssolver* solver, const char* param, int value);
    LIB_PUBLIC int ls_get_advanced_str_param(lssolver* solver, const char* param, const char* defaultValue, char* buffer, int bufferSize);
    LIB_PUBLIC void ls_set_advanced_str_param(lssolver* solver, const char* param, const char* value);
    LIB_PUBLIC double ls_get_advanced_double_param(lssolver* solver, const char* param, double defaultValue);
    LIB_PUBLIC void ls_set_advanced_double_param(lssolver* solver, const char* param, double value);
    LIB_PUBLIC void ls_set_int_objective_bound(lssolver* solver, int objPos, lsint bound);
    LIB_PUBLIC void ls_set_double_objective_bound(lssolver* solver, int objPos, lsdouble bound);
    LIB_PUBLIC lsint ls_get_int_objective_bound(lssolver* solver, int objPos);
    LIB_PUBLIC lsdouble ls_get_double_objective_bound(lssolver* solver, int objPos);

    LIB_PUBLIC int ls_create_int_constant(lssolver* solver, lsint value);
    LIB_PUBLIC int ls_create_double_constant(lssolver* solver, lsdouble value);
    LIB_PUBLIC int ls_create_expression(lssolver* solver, lsoperator op);
    LIB_PUBLIC int ls_create_native_function(lssolver* solver, lsnativefunction func, void* userdata);
    LIB_PUBLIC int ls_nb_expressions(lssolver* solver);
    LIB_PUBLIC int ls_nb_decisions(lssolver* solver);
    LIB_PUBLIC int ls_get_decision(lssolver* solver, int decisionPos);
    LIB_PUBLIC int ls_get_expression_with_name(lssolver* solver, const char* name);
    LIB_PUBLIC void ls_add_constraint(lssolver* solver, int exprId);
    LIB_PUBLIC void ls_remove_constraint(lssolver* solver, int constraintPos);
    LIB_PUBLIC void ls_remove_constraint_with_expr(lssolver* solver, int exprId);
    LIB_PUBLIC int ls_nb_constraints(lssolver* solver);
    LIB_PUBLIC int ls_get_constraint(lssolver* solver, int constraintPos);
    LIB_PUBLIC void ls_add_objective(lssolver* solver, int exprId, lsobjdirection direction);
    LIB_PUBLIC void ls_remove_objective(lssolver* solver, int objectivePos);
    LIB_PUBLIC int ls_nb_objectives(lssolver* solver);
    LIB_PUBLIC int ls_get_objective(lssolver* solver, int objectiveIndex);
    LIB_PUBLIC lsobjdirection ls_get_objective_direction(lssolver* solver, int objectivePos);
    LIB_PUBLIC int ls_nb_operands(lssolver* solver);
    LIB_PUBLIC void ls_close(lssolver* solver);
    LIB_PUBLIC void ls_open(lssolver* solver);
    LIB_PUBLIC bool ls_is_closed(lssolver* solver);
    LIB_PUBLIC int ls_model_to_string(lssolver* solver, char* buffer, int bufferSize);
    
    LIB_PUBLIC bool ls_expr_check_index(lssolver* solver, int exprIndex);
    LIB_PUBLIC lsint ls_expr_get_int_value(lssolver* solver, int exprId);
    LIB_PUBLIC lsdouble ls_expr_get_double_value(lssolver* solver, int exprId);
    LIB_PUBLIC lsarray* ls_expr_get_array_value(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_violated(lssolver* solver, int exprId);    
    LIB_PUBLIC bool ls_expr_is_undefined(lssolver* solver, int exprId);    
    LIB_PUBLIC void ls_expr_set_int_value(lssolver* solver, int exprId, lsint value);
    LIB_PUBLIC void ls_expr_set_double_value(lssolver* solver, int exprId, lsdouble value);
    LIB_PUBLIC bool ls_expr_is_named(lssolver* solver, int exprId);
    LIB_PUBLIC int ls_expr_get_name(lssolver* solver, int exprId, char* buffer, int bufferSize);
    LIB_PUBLIC void ls_expr_set_name(lssolver* solver, int exprId, const char* name);
    LIB_PUBLIC int ls_expr_info(lssolver* solver, int exprId, char* buffer, int bufferSize);
    LIB_PUBLIC int ls_expr_to_string(lssolver* solver, int exprId, char* buffer, int bufferSize);
    LIB_PUBLIC int ls_expr_nb_operands(lssolver* solver, int exprId);
    LIB_PUBLIC void ls_expr_set_operand(lssolver* solver, int exprId, int operandPos, int operandId);
    LIB_PUBLIC void ls_expr_add_operand(lssolver* solver, int exprId, int operandId);
    LIB_PUBLIC int ls_expr_get_operand(lssolver* solver, int exprId, int operandPos);
    LIB_PUBLIC bool ls_expr_is_array(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_collection(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_function(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_bool(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_int(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_double(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_objective(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_decision(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_constraint(lssolver* solver, int exprId);
    LIB_PUBLIC bool ls_expr_is_constant(lssolver* solver, int exprId);
    LIB_PUBLIC lsoperator ls_expr_get_operator(lssolver* solver, int exprId);
    
    LIB_PUBLIC int ls_array_count(lssolver* solver, lsarray* array);
    LIB_PUBLIC bool ls_array_is_bool(lssolver* solver, lsarray* array, int pos);
    LIB_PUBLIC bool ls_array_is_int(lssolver* solver, lsarray* array, int pos);
    LIB_PUBLIC bool ls_array_is_double(lssolver* solver, lsarray* array, int pos);
    LIB_PUBLIC bool ls_array_is_array(lssolver* solver, lsarray* array, int pos);
    LIB_PUBLIC lsint ls_array_get_int_value(lssolver* solver, lsarray* array, int pos);
    LIB_PUBLIC lsdouble ls_array_get_double_value(lssolver* solver, lsarray* array, int pos);
    LIB_PUBLIC lsarray* ls_array_get_array_value(lssolver* solver, lsarray* array, int pos);    
    LIB_PUBLIC int ls_array_to_string(lssolver* solver, lsarray* array, char* buffer, int bufferSize);

    LIB_PUBLIC int ls_buffer_size(lssolver* solver, lsbuffer* buffer);    
    LIB_PUBLIC bool ls_buffer_is_bool(lssolver* solver, lsbuffer* buffer, int pos);
    LIB_PUBLIC bool ls_buffer_is_int(lssolver* solver, lsbuffer* buffer, int pos);
    LIB_PUBLIC bool ls_buffer_is_double(lssolver* solver, lsbuffer* buffer, int pos);
    LIB_PUBLIC lsint ls_buffer_get_int_value(lssolver* solver, lsbuffer* buffer, int pos);
    LIB_PUBLIC lsdouble ls_buffer_get_double_value(lssolver* solver, lsbuffer* buffer, int pos);
    LIB_PUBLIC void ls_buffer_copy_int_values(lssolver* solver, lsbuffer* buffer, lsint* output, int size);
    LIB_PUBLIC void ls_buffer_copy_double_values(lssolver* solver, lsbuffer* buffer, lsdouble* output, int size);
    
    LIB_PUBLIC lssolutionstatus ls_get_solution_status(lssolver* solver);
    LIB_PUBLIC void ls_clear_solution(lssolver* solver);

    LIB_PUBLIC void ls_collection_clear(lssolver* solver, int exprId);
    LIB_PUBLIC void ls_collection_add(lssolver* solver, int exprId, lsint val);
    LIB_PUBLIC lsint ls_collection_count(lssolver* solver, int exprId);
    LIB_PUBLIC lsint ls_collection_get(lssolver* solver, int exprId, int pos);
    LIB_PUBLIC int ls_collection_to_string(lssolver* solver, int exprId, char* buffer, int bufferSize);
    
    LIB_PUBLIC int ls_cause(lssolver* solver, int causeIndex);
    LIB_PUBLIC int ls_nb_causes(lssolver* solver);        
    LIB_PUBLIC int ls_inconsistency_to_string(lssolver* solver, char* buffer, int bufferSize);    
    
    LIB_PUBLIC bool ls_phase_check_index(lssolver* solver, int phaseIndex);
    LIB_PUBLIC void ls_phase_set_optimized_objective(lssolver* solver, int phaseIndex, int optimizedObjective);
    LIB_PUBLIC int ls_phase_get_optimized_objective(lssolver* solver, int phaseIndex);
    LIB_PUBLIC void ls_phase_set_iteration_limit(lssolver* solver, int phaseIndex, long long iterationLimit);
    LIB_PUBLIC long long ls_phase_get_iteration_limit(lssolver* solver, int phaseIndex);
    LIB_PUBLIC void ls_phase_set_time_limit(lssolver* solver, int phaseIndex, int timeLimit);
    LIB_PUBLIC int ls_phase_get_time_limit(lssolver* solver, int phaseIndex);
    LIB_PUBLIC void ls_phase_set_enabled(lssolver* solver, int phaseIndex, bool enabled);
    LIB_PUBLIC bool ls_phase_is_enabled(lssolver* solver, int phaseIndex);
    LIB_PUBLIC int ls_phase_to_string(lssolver* solver, int phaseIndex, char* buffer, int bufferSize);
    
    LIB_PUBLIC int ls_stats_get_running_time(lssolver* solver, int phaseIndex);
    LIB_PUBLIC long long ls_stats_get_nb_iterations(lssolver* solver, int phaseIndex);
    LIB_PUBLIC long long ls_stats_get_nb_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC long long ls_stats_get_nb_accepted_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC long long ls_stats_get_nb_improving_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC long long ls_stats_get_nb_rejected_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC long long ls_stats_get_nb_infeasible_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC double ls_stats_get_percent_accepted_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC double ls_stats_get_percent_improving_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC double ls_stats_get_percent_rejected_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC double ls_stats_get_percent_infeasible_moves(lssolver* solver, int phaseIndex);
    LIB_PUBLIC int ls_stats_to_string(lssolver* solver, int phaseIndex, char* buffer, int bufferSize);
    LIB_PUBLIC int ls_stats_info(lssolver* solver, int phaseIndex, char* buffer, int bufferSize);
    
    LIB_PUBLIC int ls_major_version();
    LIB_PUBLIC int ls_minor_version();
    LIB_PUBLIC int ls_build_date();
    LIB_PUBLIC int ls_version_code();
    LIB_PUBLIC int ls_info(char* buffer, int bufferSize);
    LIB_PUBLIC int ls_version(char* buffer, int bufferSize);
    LIB_PUBLIC int ls_platform(char* buffer, int bufferSize);
    LIB_PUBLIC int ls_copyright(char* buffer, int bufferSize);
    LIB_PUBLIC int ls_license_path(char* buffer, int bufferSize);
    LIB_PUBLIC void ls_set_license_path(const char* path);
    LIB_PUBLIC int ls_license_content(char* buffer, int bufferSize);
    LIB_PUBLIC void ls_set_license_content(const char* content);
    LIB_PUBLIC void ls_set_exception_callback(lsexceptioncallback callback, void* userdata);
}
    
#endif
