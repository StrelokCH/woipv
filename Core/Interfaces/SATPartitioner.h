#pragma once

#include <utility>
#include <optional>

#include "Types\SolvingResult.h"
#include "Types\Assignment.h"
#include "Types\Problem.h"

class SATPartitioner {
public:
    virtual std::vector<Problem> Split(Problem p) abstract;
    virtual std::pair<SolvingResult, std::optional<Assignment>> Merge(Problem p) abstract;
};
