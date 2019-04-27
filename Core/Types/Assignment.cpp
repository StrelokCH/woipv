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

size_t Assignment::Size() const
{
    return states.size();
}

std::ostream& operator<<(std::ostream& out, const Assignment& a)
{
    for (decltype(a.Size()) i = 1; i < a.Size(); i++) {
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
