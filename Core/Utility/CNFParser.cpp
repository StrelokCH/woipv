#include "Core/stdafx.h"
#include "CNFParser.h"

#include <sstream>
#include <cctype>

#include "Core/Types/Problem.h"

/// <summary>
/// Start of the header row.
/// </summary>
const std::string header = "p cnf";

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
    Clause clause;
    while (true) {
        auto lit = ParseNumber<Literal>(input, pos);
        if (lit == 0) {
            break;
        }
        clause.push_back(lit);
    }
    return clause;
}

Problem ParseCNF(std::istream& input)
{
    if (!input) {
        return Problem();
    }

    std::string line;
    Variable numberOfVariables = 0;
    std::vector<Clause> clauses;
    size_t numberOfClauses = 0;
    size_t clausesRead = 0;
    bool foundHeader = false;
    while (std::getline(input, line)) {
        size_t pos = 0;
        SkipSpace(line, pos);
        if (!foundHeader && line.rfind(header, 0) == 0) {
            // header
            pos += header.size();
            numberOfVariables = ParseNumber<Variable>(line, pos);
            numberOfClauses = ParseNumber<Variable>(line, pos);
            foundHeader = true;
        } else if (line.rfind("c", 0) == 0) {
            // comment
        } else {
            // clause
            auto c = ParseClause(line, pos);
            if (!c.empty()) {
                clauses.push_back(c);
                clausesRead++;
            }
        }
    }

    if (numberOfClauses != clausesRead) {
        std::cout << "Wrong number of clauses. Should be " << numberOfClauses << ", was " << clausesRead << "." << std::endl;
    }

    return Problem(numberOfVariables, std::move(clauses));
}
