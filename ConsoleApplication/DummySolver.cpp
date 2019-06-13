#include "pch.h"
#include "DummySolver.h"

std::pair<SolvingResult, std::optional<Assignment>> DummySolver::Solve(const Problem & problem, OptionalTimeLimitMs timeLimit)
{
    return {SolvingResult::Undefined, {}};
}
