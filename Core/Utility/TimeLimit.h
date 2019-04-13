#pragma once

#include "Core/DLLMakro.h"

#include <optional>
#include <chrono>

using OptionalTimeLimitMs = std::optional<std::chrono::milliseconds>;

CORE_API OptionalTimeLimitMs GetRemaining(OptionalTimeLimitMs timeLimit, std::chrono::time_point<std::chrono::steady_clock> start);
CORE_API bool HasRemaining(OptionalTimeLimitMs timeLimit, std::chrono::steady_clock::time_point start);
