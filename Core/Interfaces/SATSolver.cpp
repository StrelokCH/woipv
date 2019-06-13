#include "Core/stdafx.h"
#include "SATSolver.h"

#include <algorithm>
#include <functional>

std::vector<Solution> SATSolver::Solve(const std::vector<Problem>& problems, OptionalTimeLimitMs timeLimit)
{
    auto start = std::chrono::steady_clock::now();

    decltype(Solve(problems, timeLimit)) ret;
    std::transform(problems.begin(), problems.end(), std::back_inserter(ret), [this, &start, &timeLimit](auto p) {
        return Solve(p, GetRemaining(timeLimit, start));
    });
    return ret;
}
