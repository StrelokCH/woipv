#include "Core/stdafx.h"
#include "SATSolver.h"

#include <algorithm>

std::vector<Solution> SATSolver::Solve(std::vector<Problem> problems)
{
    decltype(Solve(problems)) ret;
    std::transform(problems.begin(), problems.end(), ret.begin(), [this](auto p) {
        return Solve(p);
    });
    return ret;
}
