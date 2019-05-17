#pragma once

#include "Partitioning/DLLMakro.h"

#include "Core/Interfaces/SATPartitioner.h"

class PARTITIONINING_API AbstractPartitioner : public SATPartitioner {
private:
    std::chrono::steady_clock::time_point start;
    OptionalTimeLimitMs timeLimit;

public:
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;

protected:
    /// <summary>
    /// Must be overwritten
    /// </summary>
    /// <param name="problem"></param>
    /// <returns></returns>
    virtual Solution SolveInternal(const Problem& problem) abstract;

protected:
    virtual void CheckTimeLimit();
    virtual OptionalTimeLimitMs GetRemainingTimeLimit();
};