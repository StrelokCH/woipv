#pragma once

#include <vector>

#include "Core/DLLMakro.h"
#include "Core/Types/Problem.h"
#include "Core/Types/Solution.h"

/// <summary>
/// This class should be overwritten by concrete implementations.
/// </summary>
class CORE_API SATSolver {
public:
    virtual Solution Solve(Problem problem) abstract;
    virtual std::vector<Solution> Solve(std::vector<Problem> problems);
};
