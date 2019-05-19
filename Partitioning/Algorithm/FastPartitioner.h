#pragma once

#include "Partitioning/DLLMakro.h"

#include "AbstractPartitioner.h"

/// <summary>
/// Class to solve a problem by cube & conquer.
/// </summary>
class PARTITIONINING_API FastPartitioner : public AbstractPartitioner {
public:
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
protected:
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem) override;
    virtual bool IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet) override;
};