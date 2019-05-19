#pragma once

#include "Partitioning/DLLMakro.h"

#include "AbstractPartitioner.h"

/// <summary>
/// Class to solve problems that consist of multiple problems
/// which are connected via only one variable.
/// </summary>
class PARTITIONINING_API OnePointPartitioner : public AbstractPartitioner {
public:
    virtual Solution SolveExt(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
protected:
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem) override;
    virtual bool IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet) override;
};