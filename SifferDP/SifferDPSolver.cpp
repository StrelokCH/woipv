#include "stdafx.h"
#include "SifferDPSolver.h"

#include "SifferDP/Details/dp.h"
#include "Core/Types/Literal.h"


bool FindAssignment(const Problem& problem, const std::chrono::steady_clock::time_point& start, OptionalTimeLimitMs timeLimit, Assignment& assignment, Variable depth)
{
    if (!HasRemaining(timeLimit, start)) {
        return false;
    }

    if (depth > problem.GetNumberOfVariables()) {
        return problem.Apply(assignment) == SolvingResult::Satisfiable;
    }

    // try true
    assignment.SetState(depth, VariableState::True);
    if (FindAssignment(problem, start, timeLimit, assignment, depth + 1)) {
        return true;
    }
    // try false
    assignment.SetState(depth, VariableState::False);
    return FindAssignment(problem, start, timeLimit, assignment, depth + 1);
}

/// <summary>
/// Brute force, recursive
/// </summary>
/// <param name="problem"></param>
/// <param name="timeLimit"></param>
/// <returns></returns>
Assignment FindAssignment(const Problem& problem, const std::chrono::steady_clock::time_point& start, OptionalTimeLimitMs timeLimit)
{
    Assignment ret(problem.GetNumberOfVariables());
    if (FindAssignment(problem, start, timeLimit, ret, FirstVariable)) {
        return ret;
    }
    throw std::runtime_error("no assignment found, but there should be one");
}

std::pair<SolvingResult, std::optional<Assignment>> SifferDPSolver::Solve(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    auto start = std::chrono::steady_clock::now();

    conjunc conj;
    for (auto& clause : problem.GetClauses()) {
        disjunc disj;
        for (auto& lit : clause) {
            literal temp;
            temp.neg = lit < 0;
            temp.id = ToVariable(lit);
            disj.literals.insert(temp);
        }
        conj.disjs.push_back(std::move(disj));
    }

    SolvingResult result = SolvingResult::Undefined;
    auto sat = DPSolve(conj, timeLimit);
    std::optional<Assignment> assignment;
    if (sat.has_value()) {
        if (sat.value()) {
            result = SolvingResult::Satisfiable;
            assignment = FindAssignment(problem, start, timeLimit);
        } else {
            result = SolvingResult::Unsatisfiable;
        }
    }

    return {result, assignment};
}
