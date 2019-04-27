#pragma once

#include "DLLMakro.h"

#include "Core/Interfaces/SATSolver.h"

class GUROBI_API GurobiSolver : public SATSolver {
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
};

