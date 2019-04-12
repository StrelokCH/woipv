#pragma once

#include "Core/DLLMakro.h"
#include "SATSolver.h"

/// <summary>
/// Use this class to indicate a SATSolver that relies on SATPartitioning.
/// </summary>
class CORE_API SATPartitioner : public SATSolver {
public:
    virtual void SetPartitionSolver(SATSolver& solver) abstract;
};
