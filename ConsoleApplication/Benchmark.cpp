#include "pch.h"
#include "Programs.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

#include "CryptoMiniSat/CryptoMiniSatSolver.h"
#include "Gurobi/GurobiSolver.h"
#include "LocalSolverSat/LocalSolverSat.h"
#include "Core/Utility/CNFParser.h"
#include "Core/Utility/TimeLimit.h"

std::string GetHeader()
{
    return "time;problem;instance name;clauses;variables;density(C / V);avg clause length;min clause length;max clause length;avg number of variable occurences;min number of variable occurences;max number of variable occurences;CryptoMiniSat;CryptoMiniSat time;Gurobi;Gurobi time;LocalSolver;LocalSolver time;valid;";
}

std::string GetContent(const std::filesystem::path& path, const Problem& problem, const std::vector<SolvingResult>& results, const std::vector<std::chrono::milliseconds>& elapsed)
{
    const char Separator = ';';
    std::stringstream ret;

    // time
    {
        // warning: cancerous code
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);
        char str[26];
        ctime_s(str, sizeof str, &end_time);
        str[strnlen_s(str, 26) - 1] = '\0'; // remove \n
        ret << str << Separator;
    }

    // problem
    ret << path << Separator;

    // instance name
    ret << path.filename() << Separator;

    // clauses
    ret << problem.GetClauses().size() << Separator;

    // variables
    ret << problem.GetNumberOfVariables() << Separator;

    // density
    ret << problem.GetDensity() << Separator;

    // avg clause length
    auto clauseLength = problem.GetClauseLength();
    ret << std::get<0>(clauseLength) << Separator;

    // max clause length
    ret << std::get<1>(clauseLength) << Separator;

    // min clause length
    ret << std::get<2>(clauseLength) << Separator;

    // avg number of variable occurences
    auto varibaleOccurences = problem.GetNumberOfVariableOccurences();
    ret << std::get<0>(varibaleOccurences) << Separator;

    // max number of variable occurences
    ret << std::get<1>(varibaleOccurences) << Separator;

    // min number of variable occurences
    ret << std::get<2>(varibaleOccurences) << Separator;

    auto sat = false;
    auto unsat = false;
    for (size_t i = 0; i < results.size(); i++) {
        // result
        switch (results[i]) {
            case SolvingResult::Satisfiable:
                ret << "sat";
                sat = true;
                break;
            case SolvingResult::Unsatisfiable:
                ret << "unsat";
                unsat = true;
                break;
            case SolvingResult::Undefined:
                ret << "undef";
                break;
        }
        ret << Separator;

        // elapsed
        ret << elapsed[i].count() << Separator;
    }

    // valid
    ret << ((sat && unsat) ? 0 : 1) << Separator;

    return ret.str();
}

int Benchmark(std::string directory, std::string outputFile, OptionalTimeLimitMs timeLimitPerInstance)
{
    if (outputFile.rfind(".csv", outputFile.size() - 4) != outputFile.size() - 4) {
        throw std::runtime_error("output must be a csv file");
    }

    // setup solvers
    std::vector<std::shared_ptr<SATSolver>> solvers;
    {
        solvers.push_back(std::make_shared<CryptoMiniSatSolver>());
        solvers.push_back(std::make_shared<GurobiSolver>());
        solvers.push_back(std::make_shared<LocalSolverSat>());
    }

    // setup output
    std::ofstream outfile(outputFile, std::fstream::app);
    if (!outfile) {
        std::cout << "Could not open output file (" << outputFile << ").";
        return EXIT_FAILURE;
    }

    // write header line
    outfile << GetHeader() << std::endl;

    // solve & log
    for (auto instance : std::filesystem::recursive_directory_iterator(directory)) {
        try {
            if (!std::filesystem::is_directory(instance)) {
                std::ifstream infile(instance.path());
                if (!infile) {
                    std::cout << "Could not open input file (" << instance.path() << ").";
                    continue;
                }

                auto problem = ParseCNF(infile);

                std::vector<SolvingResult> results;
                std::vector<std::chrono::milliseconds> elapsed;
                for (auto solver : solvers) {
                    auto start = std::chrono::steady_clock::now();
                    // solve
                    results.push_back(solver->Solve(problem, timeLimitPerInstance).first);
                    // measure time
                    elapsed.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(GetElapsed(start)));
                }

                // log
                outfile << GetContent(instance.path(), problem, results, elapsed) << std::endl;
            }
        } catch (std::exception e) {
            // avoid early terminination of benchmark
            std::cout << e.what() << " instance: " << instance.path() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}