#include "Core/stdafx.h"
#include "PartialAssignment.h"

PartialAssignment::PartialAssignment()
{
}

PartialAssignment::PartialAssignment(const std::vector<Variable>& variables, VariableState state)
{
    for (const auto& variable : variables) {
        SetState(variable, state);
    }
}

PartialAssignment::PartialAssignment(const std::set<Variable>& variables, VariableState state)
{
    for (const auto& variable : variables) {
        SetState(variable, state);
    }
}

bool PartialAssignment::IsSAT(Literal literal) const
{
    auto var = ToVariable(literal);
    auto positive = IsPositive(literal);
    if (positive) {
        return GetState(var) == VariableState::True;
    } else {
        return GetState(var) == VariableState::False;
    }
}

VariableState PartialAssignment::GetState(Variable variable) const
{
    return states.at(variable);
}

void PartialAssignment::SetState(Variable variable, VariableState state)
{
    states[variable] = state;
}

bool PartialAssignment::HasState(Variable variable) const
{
    return states.find(variable) != states.end();
}

bool PartialAssignment::IsCompatible(const PartialAssignment& other) const
{
    if (this == &other) {
        return true;
    }

    for (const auto& pair : states) {
        if (!other.HasState(pair.first)) {
            continue;
        }

        auto state = pair.second;
        auto otherState = other.GetState(pair.first);
        if (state == VariableState::False && otherState == VariableState::True) {
            return false;
        }
        if (state == VariableState::True && otherState == VariableState::False) {
            return false;
        }
    }
    return true;
}

bool PartialAssignment::IsCompatible(const Assignment& other) const
{

    for (const auto& pair : states) {
        if (other.GetNumberOfVariables() < pair.first) {
            continue;
        }

        auto state = pair.second;
        auto otherState = other.GetState(pair.first);
        if (state == VariableState::False && otherState == VariableState::True) {
            return false;
        }
        if (state == VariableState::True && otherState == VariableState::False) {
            return false;
        }
    }
    return true;
}

void CreateTruthTable(
    std::vector<PartialAssignment>& ret,
    const std::vector<Variable>& variables,
    PartialAssignment& assignment,
    const size_t& depth)
{
    if (depth == variables.size()) {
        ret.push_back(assignment);
        return;
    }

    // positive
    assignment.SetState(variables[depth], VariableState::True);
    CreateTruthTable(ret, variables, assignment, depth + 1);

    // negative
    assignment.SetState(variables[depth], VariableState::False);
    CreateTruthTable(ret, variables, assignment, depth + 1);
}

std::vector<PartialAssignment> CreateTruthTable(const std::vector<Variable>& variables)
{
    std::vector<PartialAssignment> ret;
    PartialAssignment assignment(variables);
    CreateTruthTable(ret, variables, assignment, 0);
    return ret;
}

std::vector<PartialAssignment> CreateTruthTable(const std::set<Variable>& variables)
{
    return CreateTruthTable(std::vector<Variable>(variables.begin(), variables.end()));
}
