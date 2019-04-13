#pragma once

#include <vector>

#include "Core/DLLMakro.h"
#include "Core/Types/Problem.h"
#include "Core/Types/Solution.h"
#include "Core/Utility/TimeLimit.h"

/// <summary>
/// This class should be overwritten by concrete implementations.
/// </summary>
class CORE_API SATSolver {
public:
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) abstract;
    virtual std::vector<Solution> Solve(const std::vector<Problem>& problems, OptionalTimeLimitMs timeLimit);
};
