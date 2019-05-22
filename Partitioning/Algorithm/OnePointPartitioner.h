#pragma once

#include "Partitioning/DLLMakro.h"

#include "AbstractPartitioner.h"

struct Partition {
    std::vector<Clause> clauses;
    std::set<Variable> variables;

    Partition(const std::vector<Clause>& clauses, const std::set<Variable>& variables) :
        clauses(clauses), variables(variables)
    {
    }

    Partition(std::vector<Clause>&& clauses, std::set<Variable>&& variables) :
        clauses(clauses), variables(variables)
    {
    }

    Partition(Partition&& other) :
        clauses(std::move(other.clauses)), variables(std::move(other.variables))
    {
    }
    Partition& operator=(const Partition& other)
    {
        if (&other != this) {
            clauses = other.clauses;
            variables = other.variables;
        }
        return *this;
    }
    Partition& operator=(Partition&& other)
    {
        if (&other != this) {
            clauses = std::move(other.clauses);
            variables = std::move(other.variables);
        }
        return *this;
    }
};

/// <summary>
/// Class to solve problems that consist of multiple problems
/// which are connected via only one variable.
/// </summary>
class PARTITIONINING_API OnePointPartitioner : public AbstractPartitioner {
public:
    virtual Solution SolveExt(const Problem& problem, OptionalTimeLimitMs timeLimit) override;
private:
    virtual std::vector<Partition> ConvertClauses(std::vector<Clause>& clauses);
    virtual void MergePartitions(std::vector<Partition>& partitions, size_t& into, size_t& from);
    virtual void DeletePartition(std::vector<Partition>& partitions, size_t& position);
    virtual void MergePartitionsC2(std::vector<Partition>& partitions);
    virtual std::vector<Clause> MergeClauses1(std::vector<Partition>& partitions);
    virtual void MergeConnections(std::vector<Partition>& partitions);
    virtual Solution SolveSubproblems(const Problem& problem, std::vector<Partition>& partitions);
    virtual std::vector<Partition> ExtractConnections(std::vector<Partition>& partitions);
    virtual std::set<Literal> FindCutSet(const std::vector<Partition>& partitions);
    virtual std::optional<Assignment> FindMerge(const std::vector<std::vector<Assignment>>& solutionsSubProblems, const std::set<Variable>& cutSet);
    virtual bool FindMergeRecursive(const std::vector<std::vector<Assignment>>& solutionsSubProblems, const std::vector<Variable>& cutSet, size_t depth, Assignment& assignment);
    virtual void CompleteAssignment(const std::vector<std::vector<Assignment>>& solutionsSubProblems, const std::set<Variable>& cutSet, const std::vector<Partition>& partitions, Assignment& assignment);
    virtual void AddSolutionLoose(Assignment& assignment, std::vector<Clause>& looseClauses);
protected:
    virtual std::vector<std::set<Variable>> CreatePartitions(const Problem& problem) override;
    virtual bool IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet) override;
};