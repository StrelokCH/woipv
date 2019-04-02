#include "Core/stdafx.h"
#include "Problem.h"

Problem::Problem(Variable numberOfVariables, const std::vector<Clause>& clauses) :
    numberOfVariables(numberOfVariables),
    clauses(clauses)
{
}

Problem::Problem(Variable numberOfVariables, std::vector<Clause>&& clauses) :
    numberOfVariables(numberOfVariables),
    clauses(std::move(clauses))
{
}

Problem::Problem(const Problem& other) :
    numberOfVariables(other.numberOfVariables),
    clauses(other.clauses)
{
}

Problem::Problem(Problem&& other) :
    numberOfVariables(other.numberOfVariables),
    clauses(std::move(other.clauses))
{
}

Problem& Problem::operator=(const Problem& other)
{
    if (&other != this) {
        numberOfVariables = other.numberOfVariables;
        clauses = other.clauses;
    }
    return *this;
}

Problem& Problem::operator=(Problem&& other)
{
    if (&other != this) {
        numberOfVariables = other.numberOfVariables;
        clauses = std::move(other.clauses);
    }
    return *this;
}

Problem::~Problem()
{
}

SolvingResult Problem::Apply(const Assignment& assignment) const
{
    if (numberOfVariables > (signed)assignment.Size()) {
        return SolvingResult::Undefined;
    }

    for (auto& clause : clauses) {
        bool sat = false;
        for (auto& literal : clause) {
            if (assignment.IsSAT(literal)) {
                sat = true;
                break;
            }
        }
        if (!sat) {
            return SolvingResult::Unsatisfiable;
        }
    }
    return SolvingResult::Satisfiable;
}

const std::vector<Clause>& Problem::GetClauses() const
{
    return clauses;
}

const Variable& Problem::GetNumberOfVariables() const
{
    return numberOfVariables;
}
