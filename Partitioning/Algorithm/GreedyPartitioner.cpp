#include "Partitioning/stdafx.h"
#include "GreedyPartitioner.h"

#include "TimeLimitError.h"

#include <set>
#include <algorithm>
#include <map>

Solution GreedyPartitioner::Solve(const Problem& problem, OptionalTimeLimitMs optionalTimeLimit)
{
    if (!partitionSolver) {
        throw std::runtime_error("missing partition solver");
    }

    start = std::chrono::steady_clock::now();
    timeLimit = optionalTimeLimit;

    try {
        auto partitions = CreatePartitions(problem);
        auto cutSet = FindCutSet(partitions);

        // optimistic approach
        auto assignment = CreateOptimisticAssignment(problem, cutSet);
        auto result = TrySolve(problem, partitions, cutSet, assignment);
        if (result.first == SolvingResult::Satisfiable) {
            return result;
        }

        // try solve recursivly
        assignment = Assignment(problem.GetNumberOfVariables());
        return TrySolve(problem, partitions, cutSet, assignment, FirstVariable);

    } catch (TimeLimitError&) {
        // time limit exceeded
        return {SolvingResult::Undefined, {}};
    }
}

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

std::vector<std::set<Variable>> GreedyPartitioner::CreatePartitions(const Problem& problem, decltype(NumberOfPartitions) numberOfPartitions)
{
    if (problem.GetClauses().size() < 2) {
        return {};
    }

    CheckTimeLimit();

    // use sets of clauses, where clauses are sets of variables
    auto clauses = ConvertToVariableSets(problem.GetClauses());

    // find numberOfPartitions start clauses as sets of variables
    auto partitions = FindStartPartitions(clauses, numberOfPartitions);

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
    for (auto& v : l) {
        max = std::max(max, GetConnectivity(v, r));
    }
    return max;
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

std::set<Literal> GreedyPartitioner::FindCutSet(const std::vector<std::set<Variable>>& partitions)
{
    std::vector<Variable> variables;
    for (auto& partition : partitions) {
        CheckTimeLimit();
        variables.insert(variables.end(), partition.begin(), partition.end());
    }
    std::sort(variables.begin(), variables.end());
    std::set<Variable> cutSet;
    for (size_t i = 1; i < variables.size(); i++) {
        CheckTimeLimit();
        if (variables[i - 1] == variables[i]) {
            cutSet.insert(variables[i]);
            cutSet.insert(Negate(variables[i]));
        }
    }
    return cutSet;
}

Assignment GreedyPartitioner::CreateOptimisticAssignment(const Problem& problem, std::set<Variable> cutSet)
{
    // count occurences of literals
    std::map<Literal, size_t> counter;
    for (auto& clause : problem.GetClauses()) {
        for (auto& literal : clause) {
            CheckTimeLimit();
            if (cutSet.find(literal) != cutSet.end()) {
                counter.try_emplace(literal, 0);
                counter[literal]++;
            }
        }
    }

    Assignment assignment(problem.GetNumberOfVariables());
    for (auto& positiveLiteral : cutSet) {
        CheckTimeLimit();
        if (!IsPositive(positiveLiteral)) {
            continue;
        }

        auto negativeLiteral = Negate(positiveLiteral);
        counter.try_emplace(positiveLiteral, 0);
        counter.try_emplace(negativeLiteral, 0);

        auto countPositive = counter[positiveLiteral];
        auto countNegative = counter[negativeLiteral];
        if (countPositive > countNegative) {
            assignment.SetState(positiveLiteral, VariableState::True);
        } else if (countPositive < countNegative) {
            assignment.SetState(positiveLiteral, VariableState::False);
        } else if (countPositive > 0) {
            // same count
            assignment.SetState(positiveLiteral, VariableState::True);
        }
    }

    return assignment;
}

std::vector<Problem> GreedyPartitioner::CreateSubProblems(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const Assignment& assignment)
{
    auto simplifiedProblem = SimplifyClauses(problem, assignment);

    // assign each clause to a subproblem
    std::vector<std::vector<Clause>> subProblems(partitions.size());
    for (size_t i = 0; i < partitions.size(); i++) {
        for (auto& clause : simplifiedProblem) {
            for (auto& literal : clause) {
                CheckTimeLimit();
                auto& partition = partitions[i];
                if (partition.find(literal) != partition.end()
                    || partition.find(Negate(literal)) != partition.end()) {
                    subProblems[i].push_back(clause);
                    break;
                }
            }
        }
    }

    std::vector<Problem> ret;
    for (auto& subProblem : subProblems) {
        CheckTimeLimit();
        ret.emplace_back(problem.GetNumberOfVariables(), subProblem);
    }
    return ret;
}

std::vector<Clause> GreedyPartitioner::SimplifyClauses(const Problem& problem, const Assignment& assignment)
{
    auto simplifiedClauses = problem.GetClauses();
    {
        // delete clauses that are satisfied by the assignment
        size_t clauseIndex = 0;
        for (auto& clause : problem.GetClauses()) {
            auto simplifed = false;
            for (auto& literal : clause) {
                CheckTimeLimit();
                if (assignment.IsSAT(literal)) {
                    // clause can be deleted as it is satisfied
                    simplifiedClauses.erase(simplifiedClauses.begin() + clauseIndex);
                    simplifed = true;
                    break;
                }
            }
            if (!simplifed) {
                clauseIndex++;
            }
        }
    }
    return simplifiedClauses;
}

bool GreedyPartitioner::IsBadPartitioning(const std::vector<Problem>& problems)
{
    size_t counter = 0;
    for (auto& problem : problems) {
        if (problem.GetClauses().size() > 0) {
            counter++;
        }
    }
    return counter <= 1;
}

std::vector<Solution> GreedyPartitioner::InternalSolve(std::vector<Problem>& problems)
{
    CheckTimeLimit();

    Problem dummy = {problems[0].GetNumberOfVariables(), std::vector<Clause>{Clause {1, -1}}};
    for (size_t i = 0; i < problems.size(); i++) {
        if (problems[i].GetClauses().size() == 0) {
            // replace with dummy
            problems[i] = dummy;
        }
    }

    return partitionSolver->Solve(problems, GetRemainingTimeLimit());
}

Solution GreedyPartitioner::Merge(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment, const std::vector<Solution> solutions)
{
    if (solutions.size() != partitions.size()) {
        throw std::runtime_error("number of solutions equal the number of partitions");
    }

    // check if solutions are good
    for (auto& solution : solutions) {
        CheckTimeLimit();
        if (solution.first != SolvingResult::Satisfiable) {
            return {solution.first, {}};
        }
        if (!solution.second) {
            throw std::runtime_error("missing assignment for solution");
        }
    }

    auto finalAssignment = assignment;
    for (size_t i = 0; i < solutions.size(); i++) {
        auto& subAssignment = solutions[i].second.value();
        auto& partition = partitions[i];
        for (Variable variable = FirstVariable; variable <= problem.GetNumberOfVariables(); variable++) {
            if (cutSet.find(variable) != cutSet.end()) {
                // variable is in cut set and has not to be set
                continue;
            }

            if (partition.find(variable) == partition.end()) {
                // variable does not belong to this partition
                continue;
            }

            finalAssignment.SetState(variable, subAssignment.GetState(variable));
        }
    }

    return {problem.Apply(finalAssignment), finalAssignment};
}

Solution GreedyPartitioner::TrySolve(const Problem & problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment & assignment)
{
    auto problems = CreateSubProblems(problem, partitions, assignment);
    Solution result;
    if (IsBadPartitioning(problems)) {
        // partitions are bad
        // solve original problem directly
        result = partitionSolver->Solve(problem, GetRemainingTimeLimit());
    } else {
        auto solutions = InternalSolve(problems);
        result = Merge(problem, partitions, cutSet, assignment, solutions);
    }
    return result;
}

Solution GreedyPartitioner::TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, Assignment& assignment, Variable depth)
{
    CheckTimeLimit();

    if (depth > problem.GetNumberOfVariables()) {
        return {problem.Apply(assignment), assignment};
    }

    // try true
    assignment.SetState(depth, VariableState::True);
    auto result = TrySolve(problem, partitions, cutSet, assignment, depth + 1);
    if (result.first == SolvingResult::Satisfiable) {
        return result;
    }

    // try false
    assignment.SetState(depth, VariableState::False);
    return TrySolve(problem, partitions, cutSet, assignment, depth + 1);
}

void GreedyPartitioner::CheckTimeLimit()
{
    if (!HasRemaining(timeLimit, start)) {
        throw TimeLimitError();
    }
}

OptionalTimeLimitMs GreedyPartitioner::GetRemainingTimeLimit()
{
    return GetRemaining(timeLimit, start);
}
