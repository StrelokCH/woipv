#include "Core/stdafx.h"
#include "CNFWriter.h"

#include "CNFConstants.h"

void WriteCNF(const Problem& problem, std::ostream& output)
{
    // write header
    output << CNFHeader << " " << problem.GetNumberOfVariables() << " " << problem.GetClauses().size() << std::endl;

    for (auto& clause : problem.GetClauses()) {
        output << clause << std::endl;
    }
}