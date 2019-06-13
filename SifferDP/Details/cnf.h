//
// Created by Florian Siffer on 2019-03-17.
// Source: https://gitlab.fhnw.ch/florian.siffer/woipv_6id_fs19_dpsolver
//
#pragma once

#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

// #include "cnf_io/cnf_io.hpp"
// see https://people.sc.fsu.edu/~jburkardt/cpp_src/cnf_io/cnf_io.html

#pragma once

using namespace std;

class literal {
public:
    bool neg;
    int id;

    bool operator==(const literal &other) const
    {
        return id == other.id
            && neg == other.neg;
    }

    bool operator<(const literal &other) const
    {
        return id < other.id;
    }

    string toString()
    {
        string res;
        if (neg) {
            res += "not ";
        }
        res += 'x' + to_string(id);
        return res;
    }

    string toCNF()
    {
        string res;
        if (neg) {
            res += "-";
        }
        res += to_string(id);
        return res;
    }
};

class disjunc {
public:
    set<literal> literals;

    void addLits(disjunc other)
    {
        for (literal other_lit : other.literals) {
            literals.insert(other_lit);
        }
    }

    void removeLitId(int lit_id)
    {
        auto itr_to_erase = find_if(literals.begin(), literals.end(),
            [lit_id](literal lit) { return lit.id == lit_id; });

        while (itr_to_erase != literals.end()) {
            literals.erase(itr_to_erase);

            itr_to_erase = find_if(literals.begin(), literals.end(),
                [lit_id](literal lit) { return lit.id == lit_id; });

        }
    }

    void removeLit(literal lit)
    {
        auto itr_to_erase = find_if(literals.begin(), literals.end(),
            [lit](literal lit_inner) { return lit == lit_inner; });
        if (itr_to_erase != literals.end()) {
            literals.erase(itr_to_erase);
        } else {
            throw logic_error(
                "in removeLit: " + toString() + " does not contain literal with id = " + to_string(lit.id) +
                " with neg = " + to_string(lit.neg));
        }
    }

    bool negOfLitId(int lit_id)
    {
        for (literal lit : literals) {
            if (lit.id == lit_id) {
                return lit.neg;
            }
        }
        throw logic_error("in negOfLitId: " + toString() + " does not contain literal with id " + to_string(lit_id));
    }

    bool isTaut() const
    {
        for (literal lit : literals) {

            vector<literal> sameLits;
            copy_if(literals.begin(), literals.end(), back_inserter(sameLits),
                [lit](literal l) { return lit.id == l.id; });


            bool neg = false;
            bool nneg = false;
            for (literal sameLit : sameLits) {
                if (sameLit.neg) {
                    neg = true;
                } else {
                    nneg = true;
                }
            }

            if (neg && nneg) {
                return true;
            }
        }

        return false;
    }

    bool isUnitClause()
    {
        return literals.size() == 1;
    }

    bool contains(const literal check) const
    {
        for (literal lit : literals) {
            if (lit.id == check.id && lit.neg == check.neg) {
                return true;
            }
        }
        return false;
    }

    bool containsNeg(const literal check) const
    {
        for (literal lit : literals) {
            if (lit.id == check.id && lit.neg != check.neg) {
                return true;
            }
        }
        return false;
    }

    bool contains(const int check) const
    {
        for (literal lit : literals) {
            if (lit.id == check) {
                return true;
            }
        }
        return false;
    }


    const literal anyLit() const
    {
        if (literals.empty()) {
            throw runtime_error("Cannot get any literal from a disjunction containing no literals.");
        }
        return (*literals.begin());
    }

    bool operator==(const disjunc &other) const
    {
        return literals == other.literals;
    }

    bool operator<(const disjunc &other) const
    {
        return literals < other.literals;
    }

    string toString()
    {
        string res;
        for (literal lit : literals) {
            if (res.length() > 0) {
                res += " or ";
            }
            res += "(" + lit.toString() + ")";
        }
        return res;
    }

    string toCNF()
    {
        string res;

        for (literal lit : literals) {
            if (res.length() > 0) {
                res += " ";
            }
            res += lit.toCNF();
        }
        res += " 0\n";
        return res;
    }
};

class conjunc {
public:
    vector<disjunc> disjs;

    void distribute(conjunc to_distribute)
    {
        vector<disjunc> disjs_orig = disjs;
        disjs.clear();

        for (const disjunc &a_to_distribute : to_distribute.disjs) {
            if (!a_to_distribute.isTaut()) { // TODO unclear if this shortcut is allowed
                for (disjunc disj : disjs_orig) {
                    disj.addLits(a_to_distribute);
                    disjs.push_back(disj);
                }
            }
        }
    }

    void merge(conjunc to_merge)
    {
        for (const disjunc disj : to_merge.disjs) {
            disjs.push_back(disj);
        }
    }

    void removeLit(literal lit)
    {
        for (disjunc &disj : disjs) {
            if (disj.contains(lit)) {
                disj.removeLit(lit);
            }
        }
    }

    void removeLitId(int lit_id)
    {
        for (disjunc &disj : disjs) {
            disj.removeLitId(lit_id);
        }
    }

    disjunc anyDisj() const
    {
        if (disjs.empty()) {
            throw runtime_error("Cannot get any disjunction from a conjunction containing no literals.");
        }
        return (*disjs.begin());
    }

    void removeDisjs(vector<disjunc> to_remove)
    {
        for (const disjunc &dj : to_remove) {
            auto itr = remove(disjs.begin(), disjs.end(), dj);
            disjs.erase(itr, disjs.end());
        }
    }

    string toString()
    {
        string res;
        for (disjunc disj : disjs) {
            if (res.length() > 0) {
                res += " and ";
            }
            res += "(" + disj.toString() + ")";
        }
        return res;
    }

    string toCNF()
    {
        string res;
        res += "p cnf 50 " + to_string(disjs.size()) + "\n";
        for (disjunc disj : disjs) {
            res += disj.toCNF();
        }
        return res;
    }
};

conjunc LoadCNFFromPath(const string &path)
{

    conjunc res;
    ifstream file(path);
    if (!file.good()) {
        throw runtime_error("could not open stream to file at " + path);
    }

    // skip whitespace
    while (file.peek() == ' ' && !file.eof()) {
        file.get();
    }

    // skip comments
    while (file.peek() == 'c' && !file.eof()) {
        file.ignore(numeric_limits<streamsize>::max(), file.widen('\n'));
        while (file.peek() == ' ' && !file.eof()) {
            file.get();
        }
    }

    // skip header
    while (file.peek() == 'p' && !file.eof()) {
        file.ignore(numeric_limits<streamsize>::max(), file.widen('\n'));
        while (file.peek() == ' ' && !file.eof()) {
            file.get();
        }
    }

    // read F
    int lit_signed;

    disjunc disj;
    while (file >> lit_signed) {
        if (lit_signed == 0) {
            res.disjs.push_back(disj);
            disj = disjunc();

        } else {
            literal lit;
            lit.neg = lit_signed < 0;
            lit.id = abs(lit_signed);
            disj.literals.insert(lit);
        }
    }

    return res;
}
