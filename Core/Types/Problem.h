#pragma once

#include "Core/DLLMakro.h"

#include <vector>

#include "Clause.h"
#include "Literal.h"
#include "Assignment.h"
#include "SolvingResult.h"

template class CORE_API std::allocator<Clause>;
template class CORE_API std::vector<Clause>;

class CORE_API Problem {
private:
    Variable numberOfVariables = 0;
    std::vector<Clause> clauses;

public:
    Problem() = default;
    Problem(Variable numberOfVariables, const std::vector<Clause>& clauses);
    Problem(Variable numberOfVariables, std::vector<Clause>&& clauses);
    Problem(const Problem& other);
    Problem(Problem&& other);
    Problem& operator=(const Problem& other);
    Problem& operator=(Problem&& other);
    virtual ~Problem();

public:
    /// <summary>
    /// Applies the given assignment to this problem.
    /// Returns if the problem is satisfied with the assignment.
    /// Returns "Undefined" if the Assignment does not match the problem.
    /// </summary>
    /// <param name="assignment"></param>
    /// <returns></returns>
    SolvingResult Apply(const Assignment& assignment) const;

    const std::vector<Clause>& GetClauses() const;
    const Variable& GetNumberOfVariables() const;
    double GetDensity() const;
    /// <summary>
    ///
    /// </summary>
    /// <returns>avg,min,max</returns>
    std::tuple<double, size_t, size_t> GetClauseLength() const;
    /// <summary>
    ///
    /// </summary>
    /// <returns>avg,min,max</returns>
    std::tuple<double, size_t, size_t> GetNumberOfVariableOccurences() const;
};

