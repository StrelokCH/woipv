#pragma once

#include "DLLMakro.h"

#include "Core/Interfaces/SATSolver.h"

class GUROBI_API GurobiSolver : public SATSolver {
private:
    static constexpr bool ENABLE_CONSOLE_LOGGING = false;
public:
    virtual std::pair<SolvingResult, std::optional<Assignment>> Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
};

