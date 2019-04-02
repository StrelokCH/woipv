#pragma once

#include <utility>
#include <optional>

#include "Core/DLLMakro.h"
#include "Core/Types/SolvingResult.h"
#include "Core/Types/Assignment.h"
#include "Core/Types/Problem.h"

/// <summary>
/// This class should be overwritten by concrete implementations.
/// </summary>
class CORE_API SATSolver {
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(Problem p);
};
