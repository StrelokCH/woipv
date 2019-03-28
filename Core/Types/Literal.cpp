#include "stdafx.h"
#include "Literal.h"

#include <cmath>

CORE_API Variable ToVariable(Literal literal)
{
    return abs(literal);
}