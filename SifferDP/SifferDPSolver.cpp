#include "stdafx.h"
#include "SifferDPSolver.h"

#include "SifferDP/Details/dp.h"
#include "Core/Types/Literal.h"

std::pair<SolvingResult, std::optional<Assignment>> SifferDPSolver::Solve(Problem problem)
{
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
    if (DPSolve(conj)) {
        result = SolvingResult::Satisfiable;
    } else {
        result = SolvingResult::Unsatisfiable;
    }

    return std::make_pair(result, std::optional<Assignment>());
}
