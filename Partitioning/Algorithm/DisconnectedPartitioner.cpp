#include "DisconnectedPartitioner.h"

#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>

std::vector<std::set<Variable>> DisconnectedPartitioner::CreatePartitions(const Problem & problem)
{
    // build neighbour list
    std::vector<std::set<Variable>> partitions;
    partitions.emplace_back(std::set<Variable>()); // variable 0
    for (Variable i = FirstVariable; i <= problem.GetNumberOfVariables(); i++) {
        partitions.emplace_back(std::set<Variable>());
    }

    // add neighbours
    for (const auto& clause : problem.GetClauses()) {
        CheckTimeLimit();
        std::set<Variable> varClause;
        std::transform(clause.begin(), clause.end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
            return ToVariable(lit);
        });
        for (const auto& var : varClause) {
            partitions[var].insert(varClause.begin(), varClause.end());
        }
    }

    // merge partitions
    for (auto& partition : partitions) {
        if (partition.empty()) {
            continue;
        }

        bool addedConnections = false;
        do {
            addedConnections = false;
            for (const auto& var : partition) {
                CheckTimeLimit();
                auto& conntected = partitions[var];
                if (!conntected.empty() && &partition != &conntected) {
                    addedConnections = true;
                    partition.insert(conntected.begin(), conntected.end());
                    conntected.clear();
                    break;
                }
            }
        } while (addedConnections);
    }

    RemoveEmptyPartitions(partitions);

    return partitions;
}

bool DisconnectedPartitioner::IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet)
{
    return problems.size() > std::pow(2, cutSet.size());
}
