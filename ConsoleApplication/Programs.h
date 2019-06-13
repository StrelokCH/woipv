#pragma once

#include <string>

#include "Core/Interfaces/SATSolver.h"
#include "Core/Utility/TimeLimit.h"
#include "Core/Types/Literal.h"

int Benchmark(std::string directory, std::string outputFile, OptionalTimeLimitMs timeLimitPerInstance);
int SingleInstance(std::string instance, std::string outputFile, OptionalTimeLimitMs timeLimit);

/// <summary>
/// Shifts the variables of instance.
/// Shift may be negative but must be bigger than the smallest variable (negated).
/// </summary>
int VariableShift(std::string instance, std::string outputFile, Literal shift);
