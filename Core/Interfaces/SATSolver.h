#pragma once

#include <utility>
#include <optional>

#include "Types\SolvingResult.h"
#include "Types\Assignment.h"
#include "Types\Problem.h"

class SATSolver {
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(Problem p) abstract;
};
