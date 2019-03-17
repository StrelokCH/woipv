#pragma once

#include "vector"
#include "Clause.h"
#include "Literal.h"
#include "Assignment.h"
#include "SolvingResult.h"

class Problem {
private:
    Variable numberOfVariables;
    std::vector<Clause> clauses;

public:
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
};

