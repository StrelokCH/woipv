#pragma once

#include <utility>
#include <optional>

#include "Core/DLLMakro.h"
#include "Core/Types/SolvingResult.h"
#include "Core/Types/Assignment.h"
#include "Core/Types/Problem.h"

class SATPartitioner {
public:
    virtual std::vector<Problem> Split(Problem p) abstract;
    virtual std::pair<SolvingResult, std::optional<Assignment>> Merge(Problem p) abstract;
};
