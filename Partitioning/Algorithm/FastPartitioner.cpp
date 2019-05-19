#include "FastPartitioner.h"

#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "Partitioning/Utility/ClauseUtility.h"

Solution FastPartitioner::SolveExt(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    std::vector<std::vector<Clause>> subproblems;
    std::vector<std::set<Variable>> partitions;

    auto clauses = problem.GetClauses();

    auto CreateNewPartition = [&]() {
        subproblems.push_back({clauses[0]});
        std::set<Variable> varClause;
        std::transform(clauses[0].begin(), clauses[0].end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
            return ToVariable(lit);
        });
        partitions.push_back(varClause);
        clauses.erase(clauses.begin());
    };

    CreateNewPartition();

    for (size_t partition = 0; !clauses.empty(); partition++) {
        bool foundOne = false;
        for (size_t i = 0; i < clauses.size(); i++) {
            CheckTimeLimit();

            std::set<Variable> varClause;
            std::transform(clauses[i].begin(), clauses[i].end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
                return ToVariable(lit);
            });
            if (IsConnected(partitions[partition], varClause)) {
                foundOne = true;
                subproblems[partition].push_back(clauses[i]);
                partitions[partition].insert(varClause.begin(), varClause.end());
                clauses.erase(clauses.begin() + i);
                i--;
            }
        }
        if (foundOne) {
            partition--;
        } else {
            CreateNewPartition();
        }
    }

    std::vector<Problem> problems;
    for (auto& subproblem : subproblems) {
        problems.emplace_back(problem.GetNumberOfVariables(), std::move(subproblem));
    }
    subproblems.clear();

    auto solutions = partitionSolver->Solve(problems, GetRemainingTimeLimit());

    return Merge(problem, partitions, {}, {problem.GetNumberOfVariables()}, solutions);
}

std::vector<std::set<Variable>> FastPartitioner::CreatePartitions(const Problem& problem)
{
    return std::vector<std::set<Variable>>();
}

bool FastPartitioner::IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet)
{
    return true;
}
