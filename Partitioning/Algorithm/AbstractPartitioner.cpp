#include "AbstractPartitioner.h"

#include "TimeLimitError.h"

Solution AbstractPartitioner::Solve(const Problem & problem, OptionalTimeLimitMs optionalTimeLimit)
{
    start = std::chrono::steady_clock::now();
    timeLimit = optionalTimeLimit;
    return SolveInternal(problem);
}

void AbstractPartitioner::CheckTimeLimit()
{
    if (!HasRemaining(timeLimit, start)) {
        throw TimeLimitError();
    }
}

OptionalTimeLimitMs AbstractPartitioner::GetRemainingTimeLimit()
{
    return GetRemaining(timeLimit, start);
}
