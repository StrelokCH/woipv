#pragma once

#include <vector>
#include <ostream>

#include "Core/DLLMakro.h"
#include "Literal.h"

template class CORE_API std::vector<Literal>;

using Clause = std::vector<Literal>;

CORE_API std::ostream& operator<<(std::ostream& out, const Clause& clause);
