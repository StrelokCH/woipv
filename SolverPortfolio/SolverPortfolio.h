#pragma once

#include "DLLMakro.h"

#include "Core/Interfaces/SATSolver.h"
#include "CryptoMiniSat/CryptoMiniSatSolver.h"
#include "Gurobi/GurobiSolver.h"
#include "LocalSolverSat/LocalSolverSat.h"

class PORTFOLIO_API SolverPortfolio : public SATSolver {
private:
    CryptoMiniSatSolver cms;
    GurobiSolver gurobi;
    LocalSolverSat localSolver;

public:
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;

protected:
    virtual Solution SolveUnlimited(const Problem& problem);
};