#include "stdafx.h"
#include "LocalSolverSat.h"

#include "LocalSolver/localsolver.h"

std::pair<SolvingResult, std::optional<Assignment>> LocalSolverSat::Solve(const Problem & problem, OptionalTimeLimitMs timeLimit)
{
    try {
        using namespace localsolver;

        lsint weights[] = {10, 60, 30, 40, 30, 20, 20, 2};
        lsint values[] = {1, 10, 15, 40, 60, 90, 100, 15};
        lsint knapsackBound = 102;

        // Declares the optimization model.
        localsolver::LocalSolver localsolver;
        LSModel model = localsolver.getModel();

        // 0-1 decisions
        LSExpression x[8];
        for (int i = 0; i < 8; i++)
            x[i] = model.boolVar();

        // knapsackWeight <- 10*x0 + 60*x1 + 30*x2 + 40*x3 + 30*x4 + 20*x5 + 20*x6 + 2*x7;
        LSExpression knapsackWeight = model.sum();
        for (int i = 0; i < 8; i++)
            knapsackWeight += weights[i] * x[i];

        // knapsackWeight <= knapsackBound;
        model.constraint(knapsackWeight <= knapsackBound);

        // knapsackValue <- 1*x0 + 10*x1 + 15*x2 + 40*x3 + 60*x4 + 90*x5 + 100*x6 + 15*x7;
        LSExpression knapsackValue = model.sum();
        for (int i = 0; i < 8; i++)
            knapsackValue += values[i] * x[i];

        // maximize knapsackValue;
        model.maximize(knapsackValue);

        // close model, then solve
        model.close();

        // Parameterizes the solver.
        LSPhase phase = localsolver.createPhase();
        phase.setTimeLimit(1);
        localsolver.solve();

    } catch (const std::exception& e) {
        throw e;
    }

    return std::pair<SolvingResult, std::optional<Assignment>>();
}
