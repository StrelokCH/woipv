#include "OnePointPartitioner.h"

Solution OnePointPartitioner::SolveExt(const Problem & problem, OptionalTimeLimitMs timeLimit)
{
    return Solution();
}

std::vector<std::set<Variable>> OnePointPartitioner::CreatePartitions(const Problem & problem)
{
    return std::vector<std::set<Variable>>();
}

bool OnePointPartitioner::IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet)
{
    return false;
}
