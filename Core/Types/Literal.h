#pragma once

#include <cstdint>

/// <summary>
/// Shall be unsigned.
/// </summary>
using Variable = int32_t;

/// <summary>
/// May be signed to denote a negated Variable.
/// </summary>
using Literal = Variable;

Variable toVariable(Literal literal)
{
    return abs(literal);
}
