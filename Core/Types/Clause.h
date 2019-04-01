#pragma once

#include <vector>

#include "DLLMakro.h"
#include "Literal.h"

template class CORE_API std::vector<Literal>;

using Clause = std::vector<Literal>;
