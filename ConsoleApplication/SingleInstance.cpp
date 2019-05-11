#include "pch.h"
#include "Programs.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Core/Types/Assignment.h"
#include "Core/Utility/CNFParser.h"
#include "Core/Interfaces/SATSolver.h"

#include "SifferDP/SifferDPSolver.h"
#include "CryptoMiniSat/CryptoMiniSatSolver.h"
#include "Gurobi/GurobiSolver.h"
#include "LocalSolverSat/LocalSolverSat.h"

#include "Partitioning/Algorithm/GreedyPartitioner.h"

int SingleInstance(std::string instance, std::string outputFile, OptionalTimeLimitMs timeLimit)
{
    if (outputFile.rfind(".csv", outputFile.size() - 4) != -1) {
        throw std::runtime_error("output must not be a csv file");
    }

    std::ifstream infile(instance);
    if (!infile) {
        std::cout << "Could not open input file (" << instance << ").";
        return EXIT_FAILURE;
    }

    // Todo: solve
    auto p = ParseCNF(infile);
    auto a = Assignment(5);

    auto s = std::make_shared<GurobiSolver>();
    //auto s = std::make_shared<CryptoMiniSatSolver>();
    //auto s = std::make_shared<LocalSolverSat>();

    auto ret = s->Solve(p, timeLimit);

    auto part = GreedyPartitioner();
    part.SetPartitionSolver(s);

    auto[solvingResult, assignment] = part.Solve(p, timeLimit);

    // output result
    std::stringstream output;
    switch (solvingResult) {
        case SolvingResult::Satisfiable:
            output << "sat";
            break;
        case SolvingResult::Unsatisfiable:
            output << "unsat";
            break;
        case SolvingResult::Undefined:
            output << "undef";
            break;
        default:
            throw std::runtime_error("invalid SolvingResult");
    }
    output << std::endl;
    if (assignment) {
        output << assignment.value();
    } else {
        output << "no assignment";
    }
    std::cout << output.str();

    // write result to file
    std::ofstream outfile(outputFile);
    if (!outfile) {
        std::cout << "Could not open output file (" << outputFile << ").";
        return EXIT_FAILURE;
    }
    outfile << output.str();

    return EXIT_SUCCESS;
}