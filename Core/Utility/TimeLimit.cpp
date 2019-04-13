#include "Core/stdafx.h"
#include "TimeLimit.h"

OptionalTimeLimitMs GetRemaining(OptionalTimeLimitMs timeLimit, std::chrono::time_point<std::chrono::steady_clock> start)
{
    if (!timeLimit) {
        return timeLimit;
    }

    auto elapsed = start - std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(timeLimit.value() - elapsed);

}

bool HasRemaining(OptionalTimeLimitMs timeLimit, std::chrono::time_point<std::chrono::steady_clock> start)
{
    if (!timeLimit) {
        return true;
    }

    auto elapsed = start - std::chrono::steady_clock::now();
    return timeLimit.value() < elapsed;
}
