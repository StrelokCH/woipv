#include "Core/stdafx.h"
#include "Clause.h"

std::ostream& operator<<(std::ostream& out, const Clause& clause)
{
    for (auto& literal : clause) {
        out << literal << " ";
    }
    out << 0;
    return out;
}
