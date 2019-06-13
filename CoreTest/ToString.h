/// <summary>
/// File for ToString functions needed for the unit tests.
/// </summary>
#pragma once

#include "CppUnitTest.h"

#include <iterator>
#include <numeric>

#include "Core/Types/Clause.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework {

template<> inline std::wstring ToString<std::vector<Clause>>(const std::vector<Clause>& clauses)
{
    return std::accumulate(std::begin(clauses), std::end(clauses), std::wstring(),
        [](std::wstring& l, auto& r) {
        auto t = L"{" + ToString(r) + L"}";
        return l.empty() ? t : l + L"," + t;
    });
}

template<> inline std::wstring ToString<Clause>(const Clause& clause)
{
    return std::accumulate(std::begin(clause), std::end(clause), std::wstring(),
        [](std::wstring& l, auto& r) {
        return l.empty() ? ToString(r) : l + L"," + ToString(r);
    });
}

}
