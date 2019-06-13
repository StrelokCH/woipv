#pragma once

#include "Core/DLLMakro.h"

#include <istream>

#include "Core/Types/Problem.h"

/// <summary>
/// Creates an Instance of Problem from the given cnf.
/// </summary>
/// <param name="input">in dimacs/cnf</param>
/// <returns></returns>
CORE_API Problem ParseCNF(std::istream& input);