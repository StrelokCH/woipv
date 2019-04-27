#include "stdafx.h"
#include "CryptoMiniSatSolver.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>

# include "Core/Utility/CNFWriter.h"

// Todo: move exe to a more robust location
const std::string ExeName = "..\\CryptoMiniSat\\cryptominisat5-win-amd64.exe";
const std::string DefaultOptions = "--verb 0";
const std::string TimeLimitOption = "--maxtime";

std::string exec(const std::string cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() in CryptoMiniSatSolver failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string getUniqueFilename()
{
    char buffer[L_tmpnam];
    tmpnam_s(buffer, L_tmpnam);
    return std::string(buffer);
}

std::string CreateExecCommand(std::string inputFilename, OptionalTimeLimitMs timeLimit)
{
    if (inputFilename.empty()) {
        throw std::runtime_error("filename for input is empty");
    }

    constexpr auto Blank = " ";

    auto ret = ExeName;
    ret += Blank + DefaultOptions;
    if (timeLimit) {
        auto timeLimitSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeLimit.value()).count();
        if (timeLimitSeconds == 0) {
            std::cout << "warning: minimal timelimit of CryptoMiniSatSolver should be 1 second" << std::endl;
            timeLimitSeconds = 1;
        }
        ret += Blank + TimeLimitOption + Blank + std::to_string(timeLimitSeconds);
    }
    ret += Blank + inputFilename;

    return ret;
}

SolvingResult ParseSolvingResult(std::string line)
{
    constexpr auto sat = " satisfiable";
    constexpr auto unsat = " unsatisfiable";
    constexpr auto indet = " indeterminate";

    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    if (line.rfind("s ", 0) != 0) {
        throw std::runtime_error("result line must start with \"r \"");
    }

    if (line.find(sat) != std::string::npos) {
        return SolvingResult::Satisfiable;
    }
    if (line.find(unsat) != std::string::npos) {
        return SolvingResult::Unsatisfiable;
    }
    if (line.find(indet) != std::string::npos) {
        return SolvingResult::Undefined;
    }
    throw std::runtime_error("unknown result");
}

std::pair<SolvingResult, std::optional<Assignment>> ParseResult(std::string result, Variable numberOfVariables)
{
    // examples
    //s SATISFIABLE
    //v 1 -2 3
    //v 4 -5 6 0
    //
    //s UNSATISFIABLE
    //
    //s INDETERMINATE

    std::stringstream ss(result);
    std::string line;

    // result
    if (!std::getline(ss, line)) {
        throw std::runtime_error("missing result line");
    }
    auto solvingResult = ParseSolvingResult(line);
    if (solvingResult != SolvingResult::Satisfiable) {
        return {solvingResult, {}};
    }

    // parse assignment
    Assignment assignment(numberOfVariables);
    while (std::getline(ss, line, ' ')) {
        line.erase(std::remove(line.begin(), line.end(), 'v'), line.end());
        if (line.empty()) {
            continue;
        }

        std::stringstream temp(line);
        Literal lit;
        temp >> lit;
        if (lit != 0) {
            assignment.SetState(ToVariable(lit), IsPositive(lit) ? VariableState::True : VariableState::False);
        }
    }

    return {solvingResult, assignment};
}

std::pair<SolvingResult, std::optional<Assignment>> CryptoMiniSatSolver::Solve(const Problem& problem, OptionalTimeLimitMs timeLimit)
{
    auto input = getUniqueFilename();
    {
        std::ofstream tempFile(input);
        if (!tempFile) {
            throw std::runtime_error("could not create temporary cnf-file");
        }
        WriteCNF(problem, tempFile);
    }

    // RAII for file deletion
    auto fileDeleter = [](std::string* file) {
        std::remove(file->c_str());
        delete file;
    };
    std::unique_ptr<std::string, decltype(fileDeleter)> inputCleanup(new std::string(input), fileDeleter);

    auto result = exec(CreateExecCommand(input, timeLimit));
    return ParseResult(result, problem.GetNumberOfVariables());
}
