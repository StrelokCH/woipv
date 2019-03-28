#pragma once

#include <vector>

#include "DLLMakro.h"

enum class VariableState : char {
    Undefined = -1,
    False = 0,
    True = 1,
};

template class CORE_API std::allocator<VariableState>;
template class CORE_API std::vector<VariableState>;

class CORE_API Assignment : public std::vector<VariableState> {
public:
    /// <summary>
    /// Unchecked.
    /// </summary>
    /// <param name="literal"></param>
    /// <returns></returns>
    bool IsTrue(Literal literal) const
    {
        Variable var = ToVariable(literal);
        bool positive = literal == var;
        if (positive) {
            return operator[](var) == VariableState::True;
        } else {
            return operator[](var) == VariableState::False;
        }
    }
};
