#pragma once

#include <vector>
#include <ostream>

#include "Core/DLLMakro.h"
#include "Core/Types/Literal.h"

enum class VariableState : char {
    Undefined = -1,
    False = 0,
    True = 1,
};

template class CORE_API std::allocator<VariableState>;
template class CORE_API std::vector<VariableState>;

class CORE_API Assignment {
private:
    std::vector<VariableState> states;
public:
    Assignment(Variable numberOfVariables, VariableState state = VariableState::Undefined);
public:
    /// <summary>
    /// Unchecked.
    /// </summary>
    /// <param name="literal"></param>
    /// <returns></returns>
    bool IsSAT(Literal literal) const;

    /// <summary>
    /// Unchecked.
    /// </summary>
    /// <param name="variable"></param>
    /// <returns></returns>
    VariableState GetState(Variable variable) const;

    /// <summary>
    /// Unchecked.
    /// </summary>
    /// <param name="variable"></param>
    /// <returns></returns>
    void SetState(Variable variable, VariableState state);

    size_t Size() const;
};

CORE_API std::ostream& operator<<(std::ostream& out, const Assignment& a);