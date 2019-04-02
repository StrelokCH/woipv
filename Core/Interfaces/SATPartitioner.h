#pragma once

#include <utility>
#include <optional>

#include "Core/DLLMakro.h"
#include "Core/Types/SolvingResult.h"
#include "Core/Types/Assignment.h"
#include "Core/Types/Problem.h"

CORE_API class SATPartitioner {
public:
    CORE_API virtual std::vector<Problem> Split(Problem p) abstract;
    CORE_API virtual std::pair<SolvingResult, std::optional<Assignment>> Merge(Problem p) abstract;
};
