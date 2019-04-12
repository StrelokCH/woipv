#pragma once

#include <vector>
#include <optional>
#include <chrono>

#include "Core/DLLMakro.h"
#include "Core/Types/Problem.h"
#include "Core/Types/Solution.h"

using OptionalTimeLimitMs = std::optional<std::chrono::milliseconds>;

/// <summary>
/// This class should be overwritten by concrete implementations.
/// </summary>
class CORE_API SATSolver {
public:
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) abstract;
    virtual std::vector<Solution> Solve(const std::vector<std::pair<Problem&, OptionalTimeLimitMs>>& problems);
};
