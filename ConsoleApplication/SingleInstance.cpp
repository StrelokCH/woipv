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
#include "SolverPortfolio/SolverPortfolio.h"

#include "Partitioning/Algorithm/GreedyPartitioner.h"
#include "Partitioning/Algorithm/DisconnectedPartitioner.h"
#include "Partitioning/Algorithm/FastPartitioner.h"
#include "Partitioning/Algorithm/OnePointPartitioner.h"

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

    auto problem = ParseCNF(infile);

    std::shared_ptr<SATSolver> solver;
    // solver = std::make_shared<GurobiSolver>();
    solver = std::make_shared<CryptoMiniSatSolver>();
    // solver = std::make_shared<LocalSolverSat>();
    //solver = std::make_shared<SolverPortfolio>();

#if tru // use partitioning
    //auto part = std::make_shared<FastPartitioner>();
    //auto part = std::make_shared<DisconnectedPartitioner>();
    //auto part = std::make_shared<GreedyPartitioner>();
    auto part = std::make_shared<OnePointPartitioner>();
    part->SetPartitionSolver(solver);
    solver = part;
#endif

    auto[solvingResult, assignment] = solver->Solve(problem, timeLimit);

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
        // validate assignment
        if (problem.Apply(assignment.value()) != solvingResult) {
            output << "ERROR: Assignment does not give the expected solution.";
        }
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