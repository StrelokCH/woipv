#include "Partitioning/stdafx.h"
#include "OnePointPartitioner.h"

#include "Partitioning/Utility/ClauseUtility.h"

#include <algorithm>
#include <iterator>

// Todo
#include <fstream>
const char* TempFile = "instance/debug.csv";

template <class T>
void SortBySizeDesc(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end(), [](const auto& l, const auto& r) {
        return l.size() > r.size();
    });
}

Solution OnePointPartitioner::SolveExt(const Problem& problem, OptionalTimeLimitMs timeLimit)
{

    auto clauses = problem.GetClauses();
    if (clauses.empty()) {
        throw std::runtime_error("problem without clauses is forbidden");
    }

    SortBySizeDesc(clauses);

    // transform each clause into subproblem & partition
    auto partitions = ConvertClauses(clauses);

    // merge partitions if they have at least two connections
    MergePartitionsC2(partitions);

    // merge clauses, if they only have one single connection with only one other partition
    auto looseClauses = MergeClauses1(partitions);

    // merge partitions if they have at least two connections
    MergePartitionsC2(partitions);

    // add connection clauses to the smallest partition
    MergeConnections(partitions);

    // merge partitions if they have at least two connections
    MergePartitionsC2(partitions);

    // solve subproblems
    auto result = SolveSubproblems(problem, partitions);
    if (result.first == SolvingResult::Satisfiable) {
        AddSolutionLoose(result.second.value(), looseClauses);
    }

    return result;
}

std::vector<Partition> OnePointPartitioner::ConvertClauses(std::vector<Clause>& clauses)
{
    std::vector<Partition> partitions;

    while (!clauses.empty()) {
        CheckTimeLimit();

        std::set<Variable> varClause;
        std::transform(clauses[0].begin(), clauses[0].end(), std::inserter(varClause, varClause.begin()), [](auto lit) {
            return ToVariable(lit);
        });
        partitions.emplace_back(std::vector<Clause>{clauses[0]}, varClause);
        clauses.erase(clauses.begin());

    }
    return partitions;
}

void OnePointPartitioner::MergePartitions(std::vector<Partition>& partitions, size_t& into, size_t& from)
{
    // merge
    partitions[into].clauses.insert(partitions[into].clauses.end(), partitions[from].clauses.begin(), partitions[from].clauses.end());
    partitions[into].variables.insert(partitions[from].variables.begin(), partitions[from].variables.end());
    partitions.erase(partitions.begin() + from);

    // update indices
    if (from < into) {
        into--;
    }
    from--;
}

void OnePointPartitioner::DeletePartition(std::vector<Partition>& partitions, size_t & position)
{
    partitions.erase(partitions.begin() + position);
    position--;
}

void OnePointPartitioner::MergePartitionsC2(std::vector<Partition>& partitions)
{
    const size_t MinMergeConnectivity = 2;
    for (size_t i = 0; i < partitions.size(); i++) {
        for (size_t j = 0; j < partitions.size(); j++) {
            if (i == j) {
                continue;
            }
            CheckTimeLimit();
            if (GetConnectivity(partitions[i].variables, partitions[j].variables) >= MinMergeConnectivity) {
                MergePartitions(partitions, i, j);
            }
        }
    }
}

std::vector<Clause> OnePointPartitioner::MergeClauses1(std::vector<Partition>& partitions)
{
    std::vector<Clause> looseClauses;
    for (size_t i = 0; i < partitions.size(); i++) {
        if (partitions[i].clauses.size() > 1) {
            // not a single clause
            continue;
        }

        const auto& clause = partitions[i].variables;
        std::optional<size_t> candidatePartition;
        bool isConnectedClause = false;
        bool hasMany = false;
        for (size_t j = 0; j < partitions.size(); j++) {
            if (i == j) {
                continue;
            }
            CheckTimeLimit();
            if (GetConnectivity(clause, partitions[j].variables) >= 1) {
                if (candidatePartition.has_value() && !isConnectedClause) {
                    // multiple candidates -> cancel
                    hasMany = true;
                    break;
                } else {
                    candidatePartition = j;

                    // is this only a connection to another clause?
                    isConnectedClause = partitions[j].clauses.size() == 1;
                }
            }
        }
        if (!candidatePartition.has_value()) {
            // this clause is loose
            looseClauses.push_back(std::move(partitions[i].clauses[0]));
            DeletePartition(partitions, i);
        }
        if (!hasMany && candidatePartition.has_value()) {
            // merge clause into partition
            MergePartitions(partitions, candidatePartition.value(), i);
        }
    }

    return looseClauses;
}

void OnePointPartitioner::MergeConnections(std::vector<Partition>& partitions)
{
    std::sort(partitions.begin(), partitions.end(), [](const auto& l, const auto& r) {
        return l.clauses.size() < r.clauses.size();
    });

    // assumption: added connections don't dramaticly change the size of partitions
    for (size_t i = 0; i < partitions.size(); i++) {
        if (partitions[i].clauses.size() != 1) {
            // not a connection
            continue;
        }
        for (size_t j = 0; j < partitions.size(); j++) {
            CheckTimeLimit();
            if (i == j) {
                continue;
            }
            if (GetConnectivity(partitions[i].variables, partitions[j].variables) >= 1) {
                MergePartitions(partitions, j, i);
                break;
            }
        }
    }
}

Solution OnePointPartitioner::SolveSubproblems(const Problem& problem, std::vector<Partition>& partitions)
{
    auto cutSet = FindCutSet(partitions);

    if (!IsGoodPartitioning(partitions, cutSet)) {
#ifdef _DEBUG
        {
            std::ofstream outfile(TempFile, std::fstream::app);
            if (outfile) {
                outfile << problem.GetClauses().size() << ";" << -1 << std::endl;
            }
        }
#endif
        return partitionSolver->Solve(problem, GetTimeLimit());
    }

    std::sort(partitions.begin(), partitions.end(), [](const auto& l, const auto& r) {
        return l.clauses.size() < r.clauses.size();
    });

    auto centerPartition = partitions.back();
    partitions.pop_back();

    // create individual cutSets and truth tables
    std::vector<std::set<Variable>> cutSetSubProblems;
    std::vector<std::vector<PartialAssignment>> truthTables;
    for (const auto& partitition : partitions) {
        CheckTimeLimit();

        // subCutSet
        std::set<Variable> subCutSet;
        std::set_intersection(
            partitition.variables.begin(), partitition.variables.end(),
            cutSet.begin(), cutSet.end(),
            std::inserter(subCutSet, subCutSet.begin())
        );
        cutSetSubProblems.push_back(subCutSet);

        // truth tables
        truthTables.push_back(CreateTruthTable(subCutSet));
    }

    cutSet.clear();

    // give solving again the time of time limit
    auto solvingStart = std::chrono::steady_clock::now();

    std::vector<std::vector<Solution>> solutions;
    for (size_t partition = 0; partition < partitions.size(); partition++) {
        // create subproblems
        auto subProblems = CreateSubProblems(problem, partitions[partition], cutSetSubProblems[partition], truthTables[partition]);

        // solve subproblems
        auto subSolutions = partitionSolver->Solve(subProblems, GetRemaining(GetTimeLimit(), solvingStart));

        // check solutions (if there are some)
        if (std::all_of(subSolutions.begin(), subSolutions.end(), [](const auto& sol) {
            return sol.first == SolvingResult::Unsatisfiable;
        })) {
            // all unsat
            return {SolvingResult::Unsatisfiable, {}};
        }
        if (std::any_of(subSolutions.begin(), subSolutions.end(), [](const auto& sol) {
            return sol.first == SolvingResult::Undefined;
        })) {
            // some are undef
            return {SolvingResult::Undefined, {}};
        }

        // all good
        solutions.push_back(subSolutions);
    }

    // puzzle sub solutions together
    auto centerProblem = CreateCenterProblem(problem, centerPartition, cutSetSubProblems, truthTables, solutions);

    cutSetSubProblems.clear();

    // solve final problem
    auto solution = partitionSolver->Solve(centerProblem, GetRemaining(GetTimeLimit(), solvingStart));

#ifdef _DEBUG
    {
        auto solvingEnd = std::chrono::steady_clock::now();

        std::ofstream outfile(TempFile, std::fstream::app);
        if (outfile) {
            outfile << problem.GetClauses().size() << ";" << (std::chrono::duration_cast<std::chrono::milliseconds>(solvingEnd - solvingStart).count()) << std::endl;
        }
    }
#endif

    // check if there is a solution
    if (solution.first != SolvingResult::Satisfiable || !solution.second.has_value()) {
        return solution;
    }

    // add assignments of subSolutions to final assignment
    return CompleteAssignment(solution, partitions, truthTables, solutions);

    /*
    OLD code
    std::vector<std::vector<Assignment>> solutionsSubProblems;
    auto mergedAssignment = FindMerge(solutionsSubProblems, cutSet);
    if (mergedAssignment.has_value()) {
        CompleteAssignment_Old(solutionsSubProblems, cutSet, partitions, mergedAssignment.value());
        return {SolvingResult::Satisfiable, mergedAssignment};
    }
    */
}

std::vector<Problem> OnePointPartitioner::CreateSubProblems(const Problem& problem, const Partition& partition, const std::set<Variable>& subCutSet, const std::vector<PartialAssignment>& truthTable)
{
    std::vector<Problem> ret;
    for (const auto& assignment : truthTable) {
        std::vector<Clause> clauses = partition.clauses;
        for (const auto& variable : subCutSet) {
            if (assignment.GetState(variable) == VariableState::True) {
                clauses.push_back({variable});
            }
            if (assignment.GetState(variable) == VariableState::False) {
                clauses.push_back({Negate(variable)});
            }
        }
        ret.emplace_back(problem.GetNumberOfVariables(), clauses);
    }
    return ret;
}

Clause CreateClause(const std::set<Variable>& cutSet, const PartialAssignment& assignment)
{
    decltype(CreateClause(cutSet, assignment)) clause;
    for (const auto& variable : cutSet) {
        auto state = assignment.GetState(variable);
        if (state == VariableState::Undefined) {
            // should not happen
            throw std::runtime_error("illegal state");
        }

        if (state == VariableState::True) {
            // negate variables which are true
            clause.emplace_back(Negate(variable));
        } else {
            // take variable unchanged if it is false
            clause.emplace_back(variable);
        }
    }
    return clause;
}

Problem OnePointPartitioner::CreateCenterProblem(const Problem& problem, const Partition& centerPartition, const std::vector<std::set<Variable>>& subCutSet, const std::vector<std::vector<PartialAssignment>>& truthTable, const std::vector<std::vector<Solution>>& partitionSolutions)
{
    if (subCutSet.size() != truthTable.size() || truthTable.size() != partitionSolutions.size()) {
        throw std::runtime_error("wrong dimension");
    }

    auto clauses = centerPartition.clauses;
    for (size_t partition = 0; partition < partitionSolutions.size(); partition++) {
        for (size_t solution = 0; solution < partitionSolutions[partition].size(); solution++) {
            const auto& sol = partitionSolutions[partition][solution];
            if (sol.first == SolvingResult::Unsatisfiable) {
                clauses.push_back(CreateClause(subCutSet[partition], truthTable[partition][solution]));
            }
        }
    }

    return {problem.GetNumberOfVariables(), clauses};
}

Solution OnePointPartitioner::CompleteAssignment(const Solution& solution, std::vector<Partition>& partitions, const std::vector<std::vector<PartialAssignment>>& truthTable, const std::vector<std::vector<Solution>>& partitionSolutions)
{
    if (partitions.size() != truthTable.size() || truthTable.size() != partitionSolutions.size()) {
        throw std::runtime_error("wrong dimension");
    }

    auto assignment = solution.second.value();

    for (size_t partition = 0; partition < partitionSolutions.size(); partition++) {
        for (size_t solution = 0; solution < partitionSolutions[partition].size(); solution++) {
            const auto& sol = partitionSolutions[partition][solution];
            if (sol.first != SolvingResult::Satisfiable || !sol.second.has_value()) {
                continue;
            }

            if (truthTable[partition][solution].IsCompatible(assignment)) {
                // this condition shall only be true for one solution per partition

                const auto& subAssignment = sol.second.value();
                for (const auto& variable : partitions[partition].variables) {
                    assignment.SetState(variable, subAssignment.GetState(variable));
                }
                break;
            }
        }
    }

    return {solution.first, assignment};
}

std::vector<Partition> OnePointPartitioner::ExtractConnections(std::vector<Partition>& partitions)
{
    std::vector<Partition> connections;
    for (size_t i = 0; i < partitions.size(); i++) {
        if (partitions[i].clauses.size() != 1) {
            // not a connection
            continue;
        }
        CheckTimeLimit();
        connections.push_back(std::move(partitions[i]));
        DeletePartition(partitions, i);
    }
    return connections;
}

std::set<Variable> OnePointPartitioner::FindCutSet(const std::vector<Partition>& partitions)
{
    // vector of all variables (with duplicates)
    std::vector<Variable> variables;
    for (auto& partition : partitions) {
        CheckTimeLimit();
        variables.insert(variables.end(), partition.variables.begin(), partition.variables.end());
    }
    std::sort(variables.begin(), variables.end());

    // find duplicates
    std::set<Variable> cutSet;
    for (size_t i = 1; i < variables.size(); i++) {
        CheckTimeLimit();
        if (variables[i - 1] == variables[i]) {
            cutSet.insert(variables[i]);
        }
    }
    return cutSet;
}

std::optional<Assignment> OnePointPartitioner::FindMerge(const std::vector<std::vector<Assignment>>& solutionsSubProblems, const std::set<Variable>& cutSet)
{
    std::vector<Variable> cutSetVec(cutSet.begin(), cutSet.end());
    Assignment assignment(solutionsSubProblems[0][0].GetNumberOfVariables());
    if (FindMergeRecursive(solutionsSubProblems, cutSetVec, 0, assignment)) {
        return assignment;
    }
    return {};
}

bool OnePointPartitioner::FindMergeRecursive(const std::vector<std::vector<Assignment>>& solutionsSubProblems, const std::vector<Variable>& cutSet, size_t depth, Assignment& assignment)
{
    CheckTimeLimit();

    if (depth == cutSet.size()) {
        for (size_t i = 0; i < solutionsSubProblems.size(); i++) {
            bool found = false;
            for (size_t solution = 0; solution < solutionsSubProblems[i].size(); solution++) {
                if (assignment.IsCompatible(solutionsSubProblems[i][solution])) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }

    auto variable = *(cutSet.begin() + depth);
    assignment.SetState(variable, VariableState::True);
    auto ret = FindMergeRecursive(solutionsSubProblems, cutSet, depth + 1, assignment);
    if (ret) {
        return ret;
    }
    assignment.SetState(variable, VariableState::False);
    return FindMergeRecursive(solutionsSubProblems, cutSet, depth + 1, assignment);
}

void OnePointPartitioner::CompleteAssignment_Old(const std::vector<std::vector<Assignment>>& solutionsSubProblems, const std::set<Variable>& cutSet, const std::vector<Partition>& partitions, Assignment& assignment)
{
    for (size_t subProblem = 0; subProblem < partitions.size(); subProblem++) {
        // find matching solution
        std::optional<size_t> solutionIndex;
        for (size_t solution = 0; solution < solutionsSubProblems[subProblem].size(); solution++) {
            CheckTimeLimit();
            if (assignment.IsCompatible(solutionsSubProblems[subProblem][solution])) {
                solutionIndex = solution;
                break;
            }
        }
        if (!solutionIndex.has_value()) {
            throw std::runtime_error("given assignment does not match the solutions of the subproblem");
        }

        // complete assignment
        const auto& subAssignment = solutionsSubProblems[subProblem][solutionIndex.value()];
        for (auto& variable : partitions[subProblem].variables) {
            CheckTimeLimit();
            if (cutSet.find(variable) != cutSet.end()) {
                continue;
            }
            assignment.SetState(variable, subAssignment.GetState(variable));
        }
    }
}

void OnePointPartitioner::AddSolutionLoose(Assignment& assignment, std::vector<Clause>& looseClauses)
{
    for (const auto& clause : looseClauses) {
        CheckTimeLimit();
        auto lit = clause[0];
        assignment.SetState(ToVariable(lit), IsPositive(lit) ? VariableState::True : VariableState::False);
    }
}

bool OnePointPartitioner::IsGoodPartitioning(const std::vector<Partition>& partitions, const std::set<Variable>& cutSet)
{
    return partitions.size() > 1 && cutSet.size() <= partitions.size();
}

std::vector<std::set<Variable>> OnePointPartitioner::CreatePartitions(const Problem& problem)
{
    return std::vector<std::set<Variable>>();
}

bool OnePointPartitioner::IsGoodPartitioning(const std::vector<Problem>& problems, const std::vector<std::set<Variable>>& partitions, const std::set<Variable>& cutSet)
{
    return false;
}
