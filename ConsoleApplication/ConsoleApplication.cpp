// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Core/Types/Assignment.h"
#include "Core/Utility/CNFParser.h"
#include "Core/Interfaces/SATSolver.h"

#include "SifferDP/SifferDPSolver.h"

#include "CryptoMiniSat/CryptoMiniSatSolver.h"

#include "Partitioning/Algorithm/GreedyPartitioner.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cout << "Please specify path to input file (in dimacs format) and output file.";
        return EXIT_SUCCESS;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cout << "Could not open input file (" << argv[1] << ").";
        return EXIT_FAILURE;
    }

    // Todo: solve
    auto p = ParseCNF(infile);
    auto a = Assignment(5);

    auto s = std::make_shared<SifferDPSolver>();

    auto part = GreedyPartitioner();
    part.SetPartitionSolver(s);

    auto[solvingResult, assignment] = part.Solve(p, {});

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
    std::ofstream outfile(argv[2]);
    if (!outfile) {
        std::cout << "Could not open output file (" << argv[2] << ").";
        return EXIT_FAILURE;
    }
    outfile << output.str();

    return EXIT_SUCCESS;
}