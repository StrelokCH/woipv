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

protected:
    virtual Solution SolveInternal(const Problem& problem) override;

private:
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem, decltype(NumberOfPartitions) numberOfPartitions = NumberOfPartitions);

    /// <summary>
    ///
    /// </summary>
    /// <param name="problem">clauses that were taken are removed from problem</param>
    /// <returns></returns>
    virtual std::vector<std::set<Variable>> FindStartPartitions(std::set<std::set<Variable>>& clauses, decltype(NumberOfPartitions) numberOfPartitions);

    virtual void AssignClauses(std::vector<std::set<Variable>>& partitions, std::vector<std::set<Variable>>& clauses, size_t threshold);

    /// <summary>
    ///
    /// </summary>
    /// <param name="partitions"></param>
    /// <returns>all Literals that must be resolved to create a cut</returns>
    virtual std::set<Literal> FindCutSet(const std::vector<std::set<Variable>>& partitions);

    virtual Assignment CreateOptimisticAssignment(const Problem& problem, std::set<Variable> cutSet);

    virtual std::vector<Problem> CreateSubProblems(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const Assignment& assignment);
    virtual std::vector<Clause> SimplifyClauses(const Problem& problem, const Assignment& assignment);
    virtual bool IsBadPartitioning(const std::vector<Problem>& problems);
    virtual std::vector<Solution> SolveInternal(std::vector<Problem>& problems);
    virtual Solution Merge(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment, const std::vector<Solution> solutions);
    virtual Solution TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment);
    virtual Solution TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, Assignment& assignment, Variable depth);
};
