#include "stdafx.h"
#include "GurobiSolver.h"

#include "Gurobi/gurobi_c++.h"

Assignment CreateAssignment(const std::vector<GRBVar>& variables)
{
    Assignment assignment(static_cast<Variable>(variables.size() - 1));

    Variable counter = 0;
    for (auto variable : variables) {
        if (counter == 0) {
            counter++;
            continue;
        }

        auto value = variable.get(GRB_DoubleAttr_X);
        if (value == 1.0) {
            assignment.SetState(counter, VariableState::True);
        } else if (value == 0.0) {
            assignment.SetState(counter, VariableState::False);
        } else {
            throw std::runtime_error("unknown variable value");
        }
        counter++;
    }

    return assignment;
}

std::pair<SolvingResult, std::optional<Assignment>> GurobiSolver::Solve(const Problem & problem, OptionalTimeLimitMs timeLimit)
{
    try {

        // create an environment
        GRBEnv env = GRBEnv(true);
        if (!ENABLE_CONSOLE_LOGGING) {
            env.set(GRB_IntParam_LogToConsole, 0);
        }
        env.set("LogFile", "gurobi.log");
        env.start();

        // create an empty model
        GRBModel model = GRBModel(env);

        // create variables
        std::vector<GRBVar> variables;
        variables.reserve(problem.GetNumberOfVariables() + 1);
        variables.push_back(model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "dummy"));
        for (auto variable = FirstVariable; variable <= problem.GetNumberOfVariables(); variable++) {
            variables.push_back(model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x" + std::to_string(variable)));
        }

        // add constraints
        for (auto clause : problem.GetClauses()) {
            GRBLinExpr expr;
            for (auto literal : clause) {
                if (IsPositive(literal)) {
                    expr += variables[literal];
                } else {
                    expr += (1 - variables[Negate(literal)]);
                }
            }
            model.addConstr(expr >= 1);
        }

        // settings
        if (timeLimit) {
            auto timeLimitSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeLimit.value()).count();
            if (timeLimitSeconds == 0) {
                std::cout << "warning: minimal timelimit of Gurobi is 1 second" << std::endl;
                timeLimitSeconds = 1;
            }
            model.set(GRB_DoubleParam_TimeLimit, static_cast<double>(timeLimitSeconds));
        }

        // optimize model
        model.set(GRB_DoubleParam_MIPGap, 1.0);
        model.optimize();

        // return result
        auto status = model.get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL) {
            return {SolvingResult::Satisfiable, CreateAssignment(variables)};
        } else if (status == GRB_INFEASIBLE) {
            return {SolvingResult::Unsatisfiable, {}};
        } else if (status == GRB_TIME_LIMIT) {
            return {SolvingResult::Undefined, {}};
        }
        throw std::runtime_error("unknown status");

    } catch (GRBException e) {
        throw std::runtime_error(e.getMessage());
    }

    return std::pair<SolvingResult, std::optional<Assignment>>();
}
