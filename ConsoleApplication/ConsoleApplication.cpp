// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>

#include "Types\Problem.h"

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

    auto p = Problem::FromCNF(infile);
}