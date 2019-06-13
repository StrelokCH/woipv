#pragma once

#include "Core/DLLMakro.h"

#include <ostream>

#include "Core/Types/Problem.h"

/// <summary>
/// Write the given Problem to the output in dimacs/cnf.
/// </summary>
/// <param name="problem"></param>
/// <param name="output"></param>
/// <returns></returns>
CORE_API void WriteCNF(const Problem& problem, std::ostream& output);