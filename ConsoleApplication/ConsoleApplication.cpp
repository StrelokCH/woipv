// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>

#include "Programs.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cout << "Please specify path to input file (in dimacs format) and output file.";
        return EXIT_SUCCESS;
    }

#if true
    // "instance/input.cnf" "instance/solution.txt"
    return SingleInstance({argv[1]}, {argv[2]}, {});
#else
    // "C:\Test\woipv\test" "instance/solution.csv"
    return Benchmark({argv[1]}, {argv[2]}, std::chrono::milliseconds(1000 * 100));
#endif
}