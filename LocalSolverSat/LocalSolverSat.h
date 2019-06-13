#pragma once

#include "DLLMakro.h"

#include "Core/Interfaces/SATSolver.h"

class LOCALSOLVER_API LocalSolverSat : public SATSolver {
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
};

