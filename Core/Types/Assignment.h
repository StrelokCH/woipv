#pragma once

#include <vector>

enum class VariableState : char {
    Undefined = -1,
    False = 0,
    True = 1,
};

class Assignment : public std::vector<VariableState> {
public:
    /// <summary>
    /// Unchecked.
    /// </summary>
    /// <param name="literal"></param>
    /// <returns></returns>
    bool IsTrue(Literal literal) const
    {
        Variable var = toVariable(literal);
        bool positive = literal == var;
        if (positive) {
            return operator[](var) == VariableState::True;
        } else {
            return operator[](var) == VariableState::False;
        }
    }
};
