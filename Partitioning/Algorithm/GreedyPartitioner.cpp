#include "Partitioning/stdafx.h"
#include "GreedyPartitioner.h"

#include <set>
#include <algorithm>

Solution GreedyPartitioner::Solve(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    if (!partitionSolver) {
        throw std::runtime_error("missing partition solver");
    }

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    auto dividingVariables = Split(problem, timeLimit);

    return {SolvingResult::Undefined, {}};
}

std::set<Variable> ConvertToVariableSet(Clause clause)
{
    decltype(ConvertToVariableSet(clause)) ret;
    for (auto literal : clause) {
        // replace literal with variable
        ret.insert(ToVariable(literal));
    }
    return ret;
}

std::set<std::set<Variable>> ConvertToVariableSets(std::vector<Clause> clauses)
{
    decltype(ConvertToVariableSets(clauses)) ret;
    for (auto clause : clauses) {
        ret.insert(ConvertToVariableSet(clause));
    }
    return ret;
}

std::set<Variable> GreedyPartitioner::Split(const Problem& problem, OptionalTimeLimitMs timeLimit, decltype(NumberOfPartitions) numberOfPartitions)
{
    if (problem.GetClauses().size() < 2) {
        return {};
    }

    // use sets of clauses, where clauses are sets of variables
    auto clauses = ConvertToVariableSets(problem.GetClauses());

    // find numberOfPartitions start clauses as sets of variables
    auto partitions = FindStartPartitions(clauses, numberOfPartitions);

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

    return FindCutSet(partitions);
}

size_t GetConnectivity(const std::set<Variable>& l, const std::set<Variable>& r)
{
    struct Counter {
        struct value_type {
            value_type(const Variable&)
            {
            }
        };
        void push_back(const value_type&)
        {
            ++count;
        }
        size_t count = 0;
    };

    Counter c;
    std::set_intersection(l.begin(), l.end(), r.begin(), r.end(), std::back_inserter(c));
    return c.count;
}

size_t GetConnectivity(const std::vector<std::set<Variable>>& l, const std::set<Variable>& r)
{
    decltype(GetConnectivity(l, r)) max = 0;
    for (auto v : l) {
        max = std::max(max, GetConnectivity(v, r));
    }
    return max;
}

std::vector<std::set<Variable>> GreedyPartitioner::FindStartPartitions(std::set<std::set<Variable>>& clauses, decltype(NumberOfPartitions) numberOfPartitions)
{
    decltype(FindStartPartitions(clauses, numberOfPartitions)) partitions;

    for (size_t i = 0; i < numberOfPartitions; i++) {
        // most important criteria: lower connectivity
        // second most important criteria: bigger size
        size_t bestConnetivity = std::numeric_limits<size_t>::max();
        std::set<Variable> best;
        for (auto clause : clauses) {
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

std::set<Variable> GreedyPartitioner::FindCutSet(std::vector<std::set<Variable>>& partitions)
{
    std::vector<Variable> variables;
    for (auto& partition : partitions) {
        variables.insert(variables.end(), partition.begin(), partition.end());
    }
    std::sort(variables.begin(), variables.end());
    std::set<Variable> cutSet;
    for (size_t i = 1; i < variables.size(); i++) {
        if (variables[i - 1] == variables[i]) {
            cutSet.insert(variables[i]);
        }
    }
    return cutSet;
}
