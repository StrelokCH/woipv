#pragma once

#include "Core/Interfaces/SATSolver.h"

class DummySolver : public SATSolver {
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
};

