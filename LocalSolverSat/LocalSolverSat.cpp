#include "stdafx.h"
#include "LocalSolverSat.h"

#include <iostream>

#include "LocalSolver/localsolver.h"

using namespace localsolver;

Assignment CreateAssignment(const std::vector<localsolver::LSExpression>& variables)
{
    Assignment assignment(static_cast<Variable>(variables.size() - 1));

    Variable counter = 0;
    for (auto variable : variables) {
        if (counter == 0) {
            counter++;
            continue;
        }

        auto value = variable.getIntValue();
        if (value == 1) {
            assignment.SetState(counter, VariableState::True);
        } else if (value == 0) {
            assignment.SetState(counter, VariableState::False);
        } else {
            throw std::runtime_error("unknown variable value");
        }
        counter++;
    }

    return assignment;
}

std::pair<SolvingResult, std::optional<Assignment>> LocalSolverSat::Solve(const Problem & problem, OptionalTimeLimitMs timeLimit)
{
    auto start = std::chrono::steady_clock::now();

    // Declares the optimization model.
    LocalSolver localsolver;
    LSModel model = localsolver.getModel();

    // create variables
    std::vector<LSExpression> variables;
    variables.reserve(problem.GetNumberOfVariables() + 1);
    variables.push_back(model.boolVar());
    for (auto variable = FirstVariable; variable <= problem.GetNumberOfVariables(); variable++) {
        variables.push_back(model.boolVar());
    }

    // get expression for number of satisfied clauses
    LSExpression numberOfSatClauses = model.sum();
    for (auto clause : problem.GetClauses()) {
        LSExpression expr = model.sum();
        for (auto literal : clause) {
            if (IsPositive(literal)) {
                expr += variables[literal];
            } else {
                expr += (1 - variables[Negate(literal)]);
            }
        }

        // Todo: verify usefulness of this constraint
        model.addConstraint(expr >= 1.0);

        numberOfSatClauses += model.iif(expr >= 1, 1, 0);
    }

    // maximize number of sat clauses
    model.maximize(numberOfSatClauses);

    // close model, then solve
    model.close();

    // set bound, which is to satisfy all clauses
    localsolver.getParam().setObjectiveBound(0, static_cast<lsint>(problem.GetClauses().size()));

    // set time limit if needed
    timeLimit = GetRemaining(timeLimit, start);
    if (timeLimit) {
        auto timeLimitSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeLimit.value()).count();
        if (timeLimitSeconds <= 0) {
            std::cout << "warning: minimal timelimit of CryptoMiniSatSolver is 1 second" << std::endl;
            timeLimitSeconds = 1;
        }
        LSPhase phase = localsolver.createPhase();
        phase.setTimeLimit(static_cast<int>(timeLimitSeconds));
    }

    localsolver.solve();

    // check result
    auto solutionStatus = localsolver.getSolution().getStatus();
    switch (solutionStatus) {
        case SS_Inconsistent:
            return {SolvingResult::Unsatisfiable, {}};
        case SS_Infeasible:
        case SS_Feasible:
            // not finished
            return {SolvingResult::Undefined, {}};
        case SS_Optimal:
            return {SolvingResult::Satisfiable, CreateAssignment(variables)};
    }
    throw std::runtime_error("unknown solution status");
}
