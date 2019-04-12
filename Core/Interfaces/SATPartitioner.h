#pragma once

#include "Core/DLLMakro.h"
#include "SATSolver.h"

#include <memory>

/// <summary>
/// Use this class to indicate a SATSolver that relies on SATPartitioning.
/// </summary>
class CORE_API SATPartitioner : public SATSolver {
protected:
    std::shared_ptr<SATSolver> partitionSolver;

public:
    /// <summary>
    /// Inject solver which is used for solving partitions.
    /// </summary>
    /// <param name="solver"></param>
    virtual void SetPartitionSolver(std::shared_ptr<SATSolver> solver);
};
