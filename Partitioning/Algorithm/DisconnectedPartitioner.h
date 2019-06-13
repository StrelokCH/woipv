#pragma once

#include "Partitioning/DLLMakro.h"

#include "AbstractPartitioner.h"

/// <summary>
/// Class to solve a problem by cube & conquer.
/// </summary>
class PARTITIONINING_API DisconnectedPartitioner : public AbstractPartitioner {
protected:
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem) override;
    virtual bool IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet) override;
};