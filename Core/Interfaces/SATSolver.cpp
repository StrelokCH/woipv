#include "Core/stdafx.h"
#include "SATSolver.h"

std::pair<SolvingResult, std::optional<Assignment>> SATSolver::Solve(Problem p)
{
    return std::make_pair(SolvingResult::Undefined, std::optional<Assignment>());
}
