#pragma once

#include <utility>
#include <optional>

#include "DLLMakro.h"
#include "Types\SolvingResult.h"
#include "Types\Assignment.h"
#include "Types\Problem.h"

CORE_API class SATPartitioner {
public:
    CORE_API virtual std::vector<Problem> Split(Problem p) abstract;
    CORE_API virtual std::pair<SolvingResult, std::optional<Assignment>> Merge(Problem p) abstract;
};
