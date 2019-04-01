#include "Core/stdafx.h"
#include "Problem.h"

#include <sstream>
#include <cctype>
#include <iostream>

Problem::Problem(Variable numberOfVariables, const std::vector<Clause>& clauses) :
    numberOfVariables(numberOfVariables),
    clauses(clauses)
{
}

Problem::Problem(Variable numberOfVariables, std::vector<Clause>&& clauses) :
    numberOfVariables(numberOfVariables),
    clauses(std::move(clauses))
{
}

Problem::Problem(const Problem& other) :
    numberOfVariables(other.numberOfVariables),
    clauses(other.clauses)
{
}

Problem::Problem(Problem&& other) :
    numberOfVariables(other.numberOfVariables),
    clauses(std::move(other.clauses))
{
}

Problem& Problem::operator=(const Problem& other)
{
    if (&other != this) {
        numberOfVariables = other.numberOfVariables;
        clauses = other.clauses;
    }
    return *this;
}

Problem& Problem::operator=(Problem&& other)
{
    if (&other != this) {
        numberOfVariables = other.numberOfVariables;
        clauses = std::move(other.clauses);
    }
    return *this;
}

Problem::~Problem()
{
}

SolvingResult Problem::Apply(const Assignment& assignment) const
{
    if (numberOfVariables > (signed)assignment.Size()) {
        return SolvingResult::Undefined;
    }

    for (auto& clause : clauses) {
        bool sat = false;
        for (auto& literal : clause) {
            if (assignment.IsSAT(literal)) {
                sat = true;
                break;
            }
        }
        if (!sat) {
            return SolvingResult::Unsatisfiable;
        }
    }
    return SolvingResult::Satisfiable;
}

static void SkipSpace(const std::string& input, size_t &pos)
{
    while (pos < input.size() && std::isspace(input[pos])) {
        pos++;
    }
}

template <typename T>
T ParseNumber(const std::string& input, size_t& pos)
{
    SkipSpace(input, pos);

    std::string substr = input.substr(pos);
    if (substr.empty()) {
        throw std::invalid_argument("Convert error: cannot convert empty string to number");
    }

    T ret;
    std::istringstream iss(substr);
    iss >> ret;

    if (iss.fail()) {
        throw std::invalid_argument("Convert error: cannot convert string '" + substr + "' to number");
    }
    pos += (size_t)iss.tellg();

    return ret;
}

Clause ParseClause(const std::string& input, size_t& pos)
{
    Clause c;
    while (true) {
        auto lit = ParseNumber<Literal>(input, pos);
        if (lit == 0) {
            break;
        }
        c.push_back(lit);
    }
    return c;
}

Problem Problem::FromCNF(std::istream& input)
{
    if (!input) {
        return Problem();
    }

    std::string line;
    Problem p;
    size_t clauses = 0;
    size_t clausesRead = 0;
    bool foundHeader = false;
    const std::string header = "p cnf";
    while (std::getline(input, line)) {
        size_t pos = 0;
        SkipSpace(line, pos);
        if (!foundHeader && line.rfind(header, 0) == 0) {
            // header
            pos += header.size();
            p.numberOfVariables = ParseNumber<Variable>(line, pos);
            clauses = ParseNumber<Variable>(line, pos);
        } else if (line.rfind("c", 0) == 0) {
            // comment
        } else {
            // clause
            auto c = ParseClause(line, pos);
            if (!c.empty()) {
                p.clauses.push_back(c);
                clausesRead++;
            }
        }
    }

    if (clauses != clausesRead) {
        std::cout << "Wrong number of clauses. Should be " << clauses << ", was " << clausesRead << "." << std::endl;
    }

    return p;
}

const std::vector<Clause>& Problem::GetClauses() const
{
    return clauses;
}
