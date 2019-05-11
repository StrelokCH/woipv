#include "Core/stdafx.h"
#include "Problem.h"

#include <algorithm>

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

double Problem::GetDensity() const
{
    return static_cast<double>(clauses.size()) / numberOfVariables;
}

std::tuple<double, size_t, size_t> Problem::GetClauseLength() const
{
    double sum = 0.0;
    size_t min = std::numeric_limits<size_t>::max();
    size_t max = 0;

    for (const auto& clause : clauses) {
        auto size = clause.size();
        sum += size;
        min = std::min(min, size);
        max = std::max(max, size);
    }

    return {sum / clauses.size(), min, max};
}

std::tuple<double, size_t, size_t> Problem::GetNumberOfVariableOccurences() const
{
    std::vector<size_t> counter(GetNumberOfVariables(), 0);

    for (const auto& clause : clauses) {
        for (const auto& lit : clause) {
            counter[ToVariable(lit) - 1]++;
        }
    }

    double sum = 0.0;
    size_t min = std::numeric_limits<size_t>::max();
    size_t max = 0;

    for (const auto& count : counter) {
        sum += count;
        min = std::min(min, count);
        max = std::max(max, count);
    }

    return {sum / GetNumberOfVariables(), min, max};
}