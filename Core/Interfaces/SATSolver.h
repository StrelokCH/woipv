#pragma once

#include <utility>
#include <optional>

#include "DLLMakro.h"
#include "Types\SolvingResult.h"
#include "Types\Assignment.h"
#include "Types\Problem.h"

CORE_API class SATSolver {
public:
    CORE_API virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(Problem p) abstract;
};
