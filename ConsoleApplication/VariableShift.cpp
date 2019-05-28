#include "pch.h"
#include "Programs.h"

#include <fstream>
#include <iostream>


#include "Core/Utility/CNFParser.h"
#include "Core/Utility/CNFWriter.h"

int VariableShift(std::string instance, std::string outputFile, Literal shift)
{
    if (shift == 0) {
        throw std::runtime_error("shift must not be 0");
    }

    if (outputFile.rfind(".csv", outputFile.size() - 4) != -1) {
        throw std::runtime_error("output must not be a csv file");
    }

    std::ifstream infile(instance);
    if (!infile) {
        std::cout << "Could not open input file (" << instance << ").";
        return EXIT_FAILURE;
    }

    auto originalProblem = ParseCNF(infile);

    std::vector<Clause> clauses;
    for (const auto& clause : originalProblem.GetClauses()) {
        Clause temp;
        for (const auto& lit : clause) {
            auto variable = ToVariable(lit);
            if (shift < 0 && variable + shift <= 0) {
                throw std::runtime_error("shift is too small");
            }

            temp.emplace_back((IsPositive(lit) ? 1 : -1) * (variable + shift));
        }
        clauses.push_back(temp);
    }

    if (originalProblem.GetClauses().size() != clauses.size()) {
        throw std::runtime_error("size of new clauses does not match");
    }

    Problem problem(originalProblem.GetNumberOfVariables() + shift, std::move(clauses));

    std::ofstream output(outputFile);
    if (!output) {
        throw std::runtime_error("could not create temporary cnf-file");
    }
    WriteCNF(problem, output);

    return EXIT_SUCCESS;

}
