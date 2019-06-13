// SolverPortfolio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SolverPortfolio.h"

#include <algorithm>

Solution SolverPortfolio::Solve(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    if (!timeLimit) {
        return SolveUnlimited(problem);
    }

    auto start = std::chrono::steady_clock::now();
    auto timeLimitValue = timeLimit.value();
    if (timeLimit < std::chrono::milliseconds(5000)) {
        // not enough time to actually use portfolio
        // only use CryptoMiniSat
        return cms.Solve(problem, GetRemaining(timeLimit, start));
    }

    // max of (1 second and 5%) for gurobi
    {
        const double percentageGurobi = 0.05;
        auto timeLimitGurobi = std::max(std::chrono::milliseconds(1000), std::chrono::duration_cast<std::chrono::milliseconds>(timeLimitValue * percentageGurobi));
        auto result = gurobi.Solve(problem, timeLimitGurobi);
        if (result.first != SolvingResult::Undefined) {
            return result;
        }
    }

    // use rest for CryptoMiniSat
    return cms.Solve(problem, GetRemaining(timeLimit, start));
}

Solution SolverPortfolio::SolveUnlimited(const Problem& problem)
{
    // 2 second localsolver (for trivial problems)
    auto result = gurobi.Solve(problem, std::chrono::milliseconds(2000));
    if (result.first != SolvingResult::Undefined) {
        return result;
    }

    // 10 seconds gurobi
    result = gurobi.Solve(problem, std::chrono::milliseconds(10000));
    if (result.first != SolvingResult::Undefined) {
        return result;
    }

    // CryptoMiniSat
    return cms.Solve(problem, {});
}
