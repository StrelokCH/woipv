#include "Partitioning/stdafx.h"
#include "AbstractPartitioner.h"

#include <algorithm>
#include <map>

#include "TimeLimitError.h"

Solution AbstractPartitioner::Solve(const Problem& problem, OptionalTimeLimitMs optionalTimeLimit)
{
    start = std::chrono::steady_clock::now();
    timeLimit = optionalTimeLimit;

    if (!partitionSolver) {
        throw std::runtime_error("missing partition solver");
    }

    try {
        return SolveExt(problem, optionalTimeLimit);
    } catch (TimeLimitError&) {
        // time limit exceeded
        return {SolvingResult::Undefined, {}};
    }
}

Solution AbstractPartitioner::SolveExt(const Problem& problem, OptionalTimeLimitMs optionalTimeLimit)
{
    auto partitions = CreatePartitions(problem);
    RemoveEmptyPartitions(partitions);
    if (partitions.size() <= 1) {
        // no valid partitions
        return partitionSolver->Solve(problem, GetRemainingTimeLimit());
    }
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
}

std::set<Literal> AbstractPartitioner::FindCutSet(const std::vector<std::set<Variable>>& partitions)
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

Assignment AbstractPartitioner::CreateOptimisticAssignment(const Problem& problem, std::set<Variable> cutSet)
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

Solution AbstractPartitioner::TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment)
{
    auto problems = CreateSubProblems(problem, partitions, assignment);
    Solution result;
    if (!IsGoodPartitioning(problems, partitions, cutSet)) {
        // partitions are bad
        // solve original problem directly
        result = partitionSolver->Solve(problem, GetRemainingTimeLimit());
    } else {
        auto solutions = SolveInternal(problems);
        result = Merge(problem, partitions, cutSet, assignment, solutions);
    }
    return result;
}

Solution AbstractPartitioner::TrySolve(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, Assignment& assignment, Variable depth)
{
    CheckTimeLimit();

    if (depth > problem.GetNumberOfVariables()) {
        return TrySolve(problem, partitions, cutSet, assignment);
    }

    if (cutSet.find(depth) == cutSet.end()) {
        // variable is not part of the cutSet
        // -> don't assign
        return TrySolve(problem, partitions, cutSet, assignment, depth + 1);
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

std::vector<Problem> AbstractPartitioner::CreateSubProblems(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const Assignment& assignment)
{
    auto simplifiedProblem = SimplifyClauses(problem.GetClauses(), assignment);

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

std::vector<Clause> AbstractPartitioner::SimplifyClauses(const std::vector<Clause>& clauses, const Assignment& assignment)
{
    auto copy = clauses;
    SimplifyClausesDirect(copy, assignment);
    return clauses;
}

void AbstractPartitioner::SimplifyClausesDirect(std::vector<Clause>& clauses, const Assignment& assignment)
{
    // delete clauses that are satisfied by the assignment
    for (size_t i = 0; i < clauses.size(); i++) {
        auto& clause = clauses[i];
        auto simplifed = false;
        for (auto& literal : clause) {
            CheckTimeLimit();
            if (assignment.IsSAT(literal)) {
                // clause can be deleted as it is satisfied
                clauses.erase(clauses.begin() + i);
                i--;
                break;
            }
        }
    }
}

std::vector<Solution> AbstractPartitioner::SolveInternal(std::vector<Problem>& problems)
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

Solution AbstractPartitioner::Merge(const Problem& problem, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet, const Assignment& assignment, const std::vector<Solution> solutions)
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

void AbstractPartitioner::RemoveEmptyPartitions(std::vector<std::set<Variable>>& partitions)
{
    partitions.erase(std::remove_if(partitions.begin(), partitions.end(), [](auto p) {return p.empty(); }), partitions.end());
}

void AbstractPartitioner::BeforeSolve(const Problem& problem, OptionalTimeLimitMs optionalTimeLimit)
{
}

void AbstractPartitioner::CheckTimeLimit() const
{
    if (!HasRemaining(timeLimit, start)) {
        throw TimeLimitError();
    }
}

OptionalTimeLimitMs AbstractPartitioner::GetRemainingTimeLimit() const
{
    return GetRemaining(timeLimit, start);
}

OptionalTimeLimitMs AbstractPartitioner::GetTimeLimit() const
{
    return timeLimit;
}
