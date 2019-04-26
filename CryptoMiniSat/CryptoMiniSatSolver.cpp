#include "stdafx.h"
#include "CryptoMiniSatSolver.h"

std::pair<SolvingResult, std::optional<Assignment>> CryptoMiniSatSolver::Solve(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    return {SolvingResult::Satisfiable, {}};
}
