#pragma once

#include <cstdint>
#include "Core/DLLMakro.h"

/// <summary>
/// Shall be unsigned.
/// </summary>
using Variable = int32_t;

/// <summary>
/// May be signed to denote a negated Variable.
/// </summary>
using Literal = Variable;

constexpr Variable FirstVariable = 1;

CORE_API Variable ToVariable(Literal literal);
CORE_API Literal Negate(Literal literal);
CORE_API bool IsPositive(Literal literal);
