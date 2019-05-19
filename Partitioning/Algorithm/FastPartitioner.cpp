#include "FastPartitioner.h"

#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>

// Todo
#include <sstream>
#include "TimeLimitError.h"
#include "Windows.h"

template <class I1, class I2>
bool have_common_element(I1 e1, I2 e2)
{
    auto first1 = e1.begin();
    auto last1 = e1.end();
    auto first2 = e2.begin();
    auto last2 = e2.end();
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2)
            ++first1;
        else if (*first2 < *first1)
            ++first2;
        else
            return true;
    }
    return false;
}

Solution FastPartitioner::Solve(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    BeforeSolve(problem, timeLimit);

    if (!partitionSolver) {
        throw std::runtime_error("missing partition solver");
    }

    std::vector<std::vector<Clause>> subproblems;
    std::vector<std::set<Variable>> partitions;

    auto clauses = problem.GetClauses();
    {
        subproblems.push_back({clauses[0]});
        std::set<Variable> varClause;
        std::transform(clauses[0].begin(), clauses[0].end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
            return ToVariable(lit);
        });
        partitions.push_back(varClause);
        clauses.erase(clauses.begin());
    }

    for (size_t partition = 0; !clauses.empty(); partition++) {
        bool foundOne = false;
        for (size_t i = 0; i < clauses.size(); i++) {
            CheckTimeLimit();

            std::set<Variable> varClause;
            std::transform(clauses[i].begin(), clauses[i].end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
                return ToVariable(lit);
            });
            if (have_common_element(partitions[partition], varClause)) {
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
            // todo dry
            subproblems.push_back({clauses[0]});
            std::set<Variable> varClause;
            std::transform(clauses[0].begin(), clauses[0].end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
                return ToVariable(lit);
            });
            partitions.push_back(varClause);
            clauses.erase(clauses.begin());
        }
    }

    if (partitions.size() < 2) {
        // Todo: remove
        // temporary only focus on disconnected problems
        OutputDebugStringW(L"\tConnected");
        throw TimeLimitError();
    } else {

        // output result
        std::stringstream output;
        output << "\t";
        for (auto p : subproblems) {
            output << p.size() << ", ";
        }
        output << "(# clauses per partition)";

        OutputDebugStringA(output.str().c_str());
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
