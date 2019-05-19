#pragma once

#include "Partitioning/DLLMakro.h"

#include "AbstractPartitioner.h"

#include <set>
#include <vector>

/// <summary>
/// Class to solve a problem by cube & conquer.
/// </summary>
class PARTITIONINING_API GreedyPartitioner : public AbstractPartitioner {
private:
    static const size_t NumberOfPartitions = 2;

private:
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem) override;

    /// <summary>
    ///
    /// </summary>
    /// <param name="problem">clauses that were taken are removed from problem</param>
    /// <returns></returns>
    virtual std::vector<std::set<Variable>> FindStartPartitions(std::set<std::set<Variable>>& clauses, decltype(NumberOfPartitions) numberOfPartitions);
    virtual void AssignClauses(std::vector<std::set<Variable>>& partitions, std::vector<std::set<Variable>>& clauses, size_t threshold);
    virtual bool IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet) override;
};
