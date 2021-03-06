#pragma once

#include "Partitioning/DLLMakro.h"

#include <vector>
#include <set>

#include "Core/Interfaces/SATPartitioner.h"

class PARTITIONINING_API AbstractPartitioner : public SATPartitioner {
private:
    std::chrono::steady_clock::time_point start;
    OptionalTimeLimitMs timeLimit;

public:
    /// <summary>
    /// must not be overwritten
    /// override SolveExt to implement own structure
    /// or use CreatePartitions/IsGoodPartitioning to rely on default structure
    /// </summary>
    /// <param name="problem"></param>
    /// <param name="timeLimit"></param>
    /// <returns></returns>
    virtual Solution Solve(const Problem& problem, OptionalTimeLimitMs timeLimit) final override;

protected:
    /// <summary>
    /// may be overwritten to avoid using default structure
    /// </summary>
    /// <param name="problem"></param>
    /// <param name="timeLimit"></param>
    /// <returns></returns>
    virtual Solution SolveExt(const Problem& problem, OptionalTimeLimitMs timeLimit);

protected:
    /// <summary>
    /// must be overwritten and will be called by SolveExt
    /// </summary>
    /// <param name="problem"></param>
    /// <returns></returns>
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem) abstract;
    /// <summary>
    /// must be overwritten and will be called by SolveExt
    /// </summary>
    /// <param name="problems"></param>
    /// <returns></returns>
    virtual bool IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet) abstract;

protected:
    /// <summary>
    /// may be overwritten
    /// </summary>
    /// <param name="partitions"></param>
    /// <returns>all Literals that must be resolved to create a cut</returns>
    virtual std::set<Literal> FindCutSet(const std::vector<std::set<Variable>>& partitions);

    /// <summary>
    /// create assingment that has a high probability to be correct
    /// </summary>
    /// <param name="problem"></param>
    /// <param name="cutSet"></param>
    /// <returns></returns>
    virtual Assignment CreateOptimisticAssignment(const Problem& problem, std::set<Variable> cutSet);

    /// <summary>
    /// not recursive
    /// </summary>
    /// <param name="problem"></param>
    /// <param name="partitions"></param>
    /// <param name="cutSet"></param>
    /// <param name="assignment"></param>
    /// <returns></returns>
    virtual Solution TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment);

    /// /// <summary>
    /// recursive
    /// </summary>
    /// <param name="problem"></param>
    /// <param name="partitions"></param>
    /// <param name="cutSet"></param>
    /// <param name="assignment"></param>
    /// <param name="depth"></param>
    /// <returns></returns>
    virtual Solution TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, Assignment& assignment, Variable depth);

    virtual std::vector<Problem> CreateSubProblems(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const Assignment& assignment);
    virtual std::vector<Clause> SimplifyClauses(const std::vector<Clause>& clauses, const Assignment& assignment);
    virtual void SimplifyClausesDirect(std::vector<Clause>& clauses, const Assignment& assignment);
    virtual std::vector<Solution> SolveInternal(std::vector<Problem>& problems);
    virtual Solution Merge(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment, const std::vector<Solution> solutions);
    void RemoveEmptyPartitions(std::vector<std::set<Variable>>& partitions);
    virtual void BeforeSolve(const Problem& problem, OptionalTimeLimitMs timeLimit);
protected:
    virtual void CheckTimeLimit() const;
    virtual OptionalTimeLimitMs GetRemainingTimeLimit() const;
    virtual OptionalTimeLimitMs GetTimeLimit() const;
};