#include "Core/stdafx.h"
#include "Literal.h"

#include <cmath>

Variable ToVariable(Literal literal)
{
    return abs(literal);
}

Literal Negate(Literal literal)
{
    return -literal;
}

bool IsPositive(Literal literal)
{
    return literal > 0;
}
