#pragma once

#include <set>
#include <vector>

/// <summary>
/// return how many elements the sets have in common
/// </summary>
template <class T>
size_t GetConnectivity(const std::set<T>& l, const std::set<T>& r)
{
    struct Counter {
        struct value_type {
            value_type(const T&)
            {
            }
        };
        void push_back(const value_type&)
        {
            ++count;
        }
        size_t count = 0;
    };

    Counter c;
    std::set_intersection(l.begin(), l.end(), r.begin(), r.end(), std::back_inserter(c));
    return c.count;
}

/// <summary>
/// Returns how many elements of r are in one of the sets of l
/// </summary>
template <class T>
size_t GetConnectivity(const std::vector<std::set<T>>& l, const std::set<T>& r)
{
    decltype(GetConnectivity(l, r)) max = 0;
    for (auto& v : l) {
        max = std::max(max, GetConnectivity(v, r));
    }
    return max;
}

/// <summary>
/// returns true if both sets have at leaste one common element
/// retruns false otherwise
/// </summary>
template <class T>
bool IsConnected(const std::set<T>& l, const std::set<T>& r)
{
    auto first1 = l.begin();
    auto last1 = l.end();
    auto first2 = r.begin();
    auto last2 = r.end();
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2)
            ++first1;
        else if (*first2 < *first1)
            ++first2;
        else
            return true;
    }
    return false;
}