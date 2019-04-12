#pragma once

#include "Partitioning/DLLMakro.h"

#include "Core/Interfaces/SATPartitioner.h"

#include <set>
#include <vector>

/// <summary>
/// Class to solve a problem by cube & conquer.
/// </summary>
class PARTITIONINING_API GreedyPartitioner : public SATPartitioner {
private:
    static const size_t NumberOfPartitions = 2;

public:
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) override;

private:
    virtual std::set<Variable> Split(const Problem& problem, OptionalTimeLimitMs timeLimit, decltype(NumberOfPartitions) numberOfPartitions = NumberOfPartitions);

    /// <summary>
    ///
    /// </summary>
    /// <param name="problem">clauses that were taken are removed from problem</param>
    /// <returns></returns>
    virtual std::vector<std::set<Variable>> FindStartPartitions(std::set<std::set<Variable>>& clauses, decltype(NumberOfPartitions) numberOfPartitions);

    virtual void AssignClauses(std::vector<std::set<Variable>>& partitions, std::vector<std::set<Variable>>& clauses, size_t threshold);
    virtual std::set<Variable> FindCutSet(std::vector<std::set<Variable>>& partitions);
};
