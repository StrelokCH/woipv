// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <fstream>

#include "Core/Types/Assignment.h"
#include "Core/Utility/CNFParser.h"
#include "Core/Interfaces/SATSolver.h"

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

    auto assignment = std::optional<Assignment>();
    auto solvingResult = SolvingResult::Undefined;

    std::ofstream outfile(argv[2]);
    if (!outfile) {
        std::cout << "Could not open output file (" << argv[2] << ").";
        return EXIT_FAILURE;
    }

    // output result
    switch (solvingResult) {
        case SolvingResult::Satisfiable:
            outfile << "sat";
            break;
        case SolvingResult::Unsatisfiable:
            outfile << "unsat";
            break;
        case SolvingResult::Undefined:
            outfile << "undef";
            break;
        default:
            throw std::runtime_error("invalid SolvingResult");
    }
    outfile << std::endl;
    if (assignment) {
        outfile << assignment.value();
    } else {
        outfile << "not assignment";
    }

    return EXIT_SUCCESS;
}