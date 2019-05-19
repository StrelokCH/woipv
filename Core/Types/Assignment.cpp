#include "Core/stdafx.h"
#include "Assignment.h"

#include <string>

Assignment::Assignment(Variable numberOfVariables, VariableState state)
    :
    states(std::vector<VariableState>(numberOfVariables + 1, state))
{

}

bool Assignment::IsSAT(Literal literal) const
{
    Variable var = ToVariable(literal);
    bool positive = IsPositive(literal);
    if (positive) {
        return states[var] == VariableState::True;
    } else {
        return states[var] == VariableState::False;
    }
}

VariableState Assignment::GetState(Variable variable) const
{
    return states[variable];
}

void Assignment::SetState(Variable variable, VariableState state)
{
    states[variable] = state;
}

bool Assignment::IsCompatible(const Assignment & other) const
{
    if (this == &other) {
        return true;
    }
    if (GetNumberOfVariables() != other.GetNumberOfVariables()) {
        return false;
    }

    for (size_t i = 0; i < states.size(); i++) {
        if (states[i] == VariableState::False && other.states[i] == VariableState::True) {
            return false;
        }
        if (states[i] == VariableState::True && other.states[i] == VariableState::False) {
            return false;
        }
    }
    return true;
}

Variable Assignment::GetNumberOfVariables() const
{
    return static_cast<Variable>(states.size() - 1);
}

std::ostream& operator<<(std::ostream& out, const Assignment& a)
{
    for (decltype(a.GetNumberOfVariables()) i = 1; i <= a.GetNumberOfVariables(); i++) {
        switch (a.GetState(static_cast<Variable>(i))) {
            case VariableState::Undefined:
                out << '?' << i;
                break;
            case VariableState::False:
                out << '-';
                // falltrough intended
            case VariableState::True:
                out << i;
                break;
            default:
                throw std::runtime_error("illegal state of variable");
        }
        out << " ";
    }
    return out;
}
