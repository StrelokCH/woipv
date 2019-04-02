//
// Created by Florian Siffer on 2019-03-17.
// Source: https://gitlab.fhnw.ch/florian.siffer/woipv_6id_fs19_dpsolver
//
#pragma once

#include "cnf.h"

#include <algorithm>
#include <iterator>

namespace DP_SOLVER {
bool checkNoClause(const conjunc &problem)
{
    return problem.disjs.empty();
}

bool checkAnyEmptyClause(const conjunc &problem)
{
    for (const disjunc &disj : problem.disjs) {
        if (disj.literals.empty()) {
            return true;
        }
    }
    return false;
}

void removeTauts(conjunc &problem)
{
    for (auto iter = problem.disjs.begin(); iter != problem.disjs.end();) {
        if ((*iter).isTaut()) {
            iter = problem.disjs.erase(iter);
        } else {
            ++iter;
        }
    }
}

literal selectAnyLiteral(const conjunc &problem)
{
    return problem.anyDisj().anyLit();
}

bool unitPropagation(conjunc &problem)
{
    for (disjunc disj : problem.disjs) {
        if (disj.isUnitClause()) {
            literal unit = disj.anyLit();

            // unit subsumption
            for (auto iter = problem.disjs.begin(); iter != problem.disjs.end();) {
                if ((*iter).contains(unit)) {
                    iter = problem.disjs.erase(iter);
                } else {
                    ++iter;
                }
            }

            // unit resolution
            for (disjunc &disj_ur : problem.disjs) {
                for (auto iter = disj_ur.literals.begin(); iter != disj_ur.literals.end();) {
                    if ((*iter).id == unit.id && (*iter).neg != unit.neg) {
                        iter = disj_ur.literals.erase(iter);
                    } else {
                        ++iter;
                    }
                }
            }

            return true;
        }
    }

    return false;
}

int checkSolved_checkOnly(const conjunc &problem)
{
    if (checkNoClause(problem)) {
        return 1;
    } else if (checkAnyEmptyClause(problem)) {
        return -1;
    }

    return 0;
}

// 1 = sat, -1 = unsat, 0 = continue
int checkSolved(conjunc &problem)
{
    removeTauts(problem);

    int solved = checkSolved_checkOnly(problem);
    if (solved != 0) {
        return solved;
    }

    while (unitPropagation(problem)) {
        solved = checkSolved_checkOnly(problem);
        if (solved != 0) {
            return solved;
        }
    }

    return 0;
}
}


using namespace DP_SOLVER;

static bool DPSolve(conjunc &problem)
{
    int i = 0;
    while (true) { // TODO this might be incorrect
        int solved = checkSolved(problem);
        if (solved != 0) {
            return solved == 1;
        }

        literal to_resolve = selectAnyLiteral(problem);

        conjunc conj_of_disjs_w_to_resolve;
        copy_if(problem.disjs.begin(), problem.disjs.end(),
            inserter(conj_of_disjs_w_to_resolve.disjs, conj_of_disjs_w_to_resolve.disjs.begin()),
            [to_resolve](disjunc disj) { return disj.contains(to_resolve); });

        conjunc conj_of_disjs_w_to_resolve_neg;
        copy_if(problem.disjs.begin(), problem.disjs.end(),
            inserter(conj_of_disjs_w_to_resolve_neg.disjs, conj_of_disjs_w_to_resolve_neg.disjs.begin()),
            [to_resolve](disjunc disj) { return disj.containsNeg(to_resolve); });

        problem.removeDisjs(conj_of_disjs_w_to_resolve.disjs);
        problem.removeDisjs(conj_of_disjs_w_to_resolve_neg.disjs);

        if (!(conj_of_disjs_w_to_resolve.disjs.empty() || conj_of_disjs_w_to_resolve_neg.disjs.empty())) {

            conj_of_disjs_w_to_resolve.removeLitId(to_resolve.id);
            conj_of_disjs_w_to_resolve_neg.removeLitId(to_resolve.id);

            conj_of_disjs_w_to_resolve.distribute(conj_of_disjs_w_to_resolve_neg);

            problem.merge(conj_of_disjs_w_to_resolve);
        }

        //std::cout << "After " << ++i << " iteration(s): " << problem.disjs.size() << " clauses." << endl;

    }
}
