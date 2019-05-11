#pragma once

#include <string>

#include "Core/Interfaces/SATSolver.h"

int Benchmark(std::string directory, std::string outputFile, OptionalTimeLimitMs timeLimitPerInstance);
int SingleInstance(std::string instance, std::string outputFile, OptionalTimeLimitMs timeLimit);