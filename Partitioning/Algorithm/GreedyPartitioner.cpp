#include "Partitioning/stdafx.h"
#include "GreedyPartitioner.h"

#include "TimeLimitError.h"
#include "Partitioning/Utility/ClauseUtility.h"

#include <set>
#include <algorithm>
#include <map>

std::set<Variable> ConvertToVariableSet(Clause clause)
{
    decltype(ConvertToVariableSet(clause)) ret;
    for (auto& literal : clause) {
        // replace literal with variable
        ret.insert(ToVariable(literal));
    }
    return ret;
}

std::set<std::set<Variable>> ConvertToVariableSets(std::vector<Clause> clauses)
{
    decltype(ConvertToVariableSets(clauses)) ret;
    for (auto& clause : clauses) {
        ret.insert(ConvertToVariableSet(clause));
    }
    return ret;
}

std::vector<std::set<Variable>> GreedyPartitioner::CreatePartitions(const Problem& problem)
{
    if (problem.GetClauses().size() < 2) {
        return {};
    }

    CheckTimeLimit();

    // use sets of clauses, where clauses are sets of variables
    auto clauses = ConvertToVariableSets(problem.GetClauses());

    // find numberOfPartitions start clauses as sets of variables
    auto partitions = FindStartPartitions(clauses, NumberOfPartitions);

    CheckTimeLimit();

    // order remaining clauses by size (desc)
    std::vector<std::set<Variable>> orderedClauses(clauses.begin(), clauses.end());
    std::sort(orderedClauses.begin(), orderedClauses.begin(),
        [](auto l, auto r) {
        return l.size() > r.size();
    });

    // assign the ones that have a connection
    AssignClauses(partitions, orderedClauses, 1);
    // assign all
    AssignClauses(partitions, orderedClauses, 0);

    return partitions;
}

std::vector<std::set<Variable>> GreedyPartitioner::FindStartPartitions(std::set<std::set<Variable>>& clauses, decltype(NumberOfPartitions) numberOfPartitions)
{
    decltype(FindStartPartitions(clauses, numberOfPartitions)) partitions;

    for (size_t i = 0; i < numberOfPartitions; i++) {

        CheckTimeLimit();

        // most important criteria: lower connectivity
        // second most important criteria: bigger size
        size_t bestConnetivity = std::numeric_limits<size_t>::max();
        std::set<Variable> best;
        for (auto& clause : clauses) {
            auto bigger = best.size() < clause.size();
            auto connectivity = GetConnectivity(best, clause);
            if (connectivity < bestConnetivity || (connectivity == bestConnetivity && bigger)) {
                best = clause;
                bestConnetivity = connectivity;
            }
        }

        clauses.erase(best);
        partitions.push_back(best);
    }

    return partitions;
}

void GreedyPartitioner::AssignClauses(std::vector<std::set<Variable>>& partitions, std::vector<std::set<Variable>>& clauses, size_t threshold)
{
    for (size_t i = 0; i < clauses.size(); i++) {
        CheckTimeLimit();
        auto clause = clauses[i];
        size_t max = 0;
        std::sort(partitions.begin(), partitions.end(), [&clause, &max](auto l, auto r) {
            auto c1 = GetConnectivity(clause, l);
            auto c2 = GetConnectivity(clause, r);
            max = std::max({max, c1, c2});
            return c1 == c2 ? (l.size() < r.size()) : (c1 > c2);
        });
        if (max >= threshold) {
            clauses.erase(clauses.begin() + i);
            i--;
            partitions[0].insert(clause.begin(), clause.end());
        }
    }
}

bool GreedyPartitioner::IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet)
{
    size_t counter = 0;
    for (auto& problem : problems) {
        if (problem.GetClauses().size() > 0) {
            counter++;
        }
    }
    return counter >= 2;
}
