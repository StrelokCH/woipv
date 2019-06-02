#pragma once

#include <map>
#include <set>

#include "Core/DLLMakro.h"
#include "Core/Types/Assignment.h"

class CORE_API PartialAssignment {
private:
    std::map<Variable, VariableState> states;
public:
    PartialAssignment();
    PartialAssignment(const std::vector<Variable>& variables, VariableState state = VariableState::Undefined);
    PartialAssignment(const std::set<Variable>& variables, VariableState state = VariableState::Undefined);
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

    /// <summary>
    /// Unchecked.
    /// </summary>
    /// <param name="variable"></param>
    /// <returns></returns>
    bool HasState(Variable variable) const;

    /// <summary>
    /// Checked.
    /// </summary>
    /// <param name="variable"></param>
    /// <returns></returns>
    bool IsCompatible(const PartialAssignment& other) const;

    /// <summary>
    /// Checked.
    /// </summary>
    /// <param name="variable"></param>
    /// <returns></returns>
    bool IsCompatible(const Assignment& other) const;
};

CORE_API std::vector<PartialAssignment> CreateTruthTable(const std::vector<Variable>& variables);
CORE_API std::vector<PartialAssignment> CreateTruthTable(const std::set<Variable>& variables);
