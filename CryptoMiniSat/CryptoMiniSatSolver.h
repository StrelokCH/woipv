#pragma once

#include "DLLMakro.h"

#include "Core/Interfaces/SATSolver.h"

class CMS_API CryptoMiniSatSolver : public SATSolver {
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
};

