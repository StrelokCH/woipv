#include "Core/stdafx.h"
#include "TimeLimit.h"

std::chrono::nanoseconds GetElapsed(std::chrono::steady_clock::time_point start)
{
    return std::chrono::steady_clock::now() - start;
}

OptionalTimeLimitMs GetRemaining(OptionalTimeLimitMs timeLimit, std::chrono::steady_clock::time_point start)
{
    if (!timeLimit) {
        return timeLimit;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(timeLimit.value() - GetElapsed(start));

}

bool HasRemaining(OptionalTimeLimitMs timeLimit, std::chrono::steady_clock::time_point start)
{
    if (!timeLimit) {
        return true;
    }

    return timeLimit.value() > GetElapsed(start);
}
