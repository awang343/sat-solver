#include "dpll.h"
#include "sat_instance.h"
#include "types.h"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

Solver::Solver() : instance() {}

void Solver::setInstance(SATInstance &instance) { this->instance = &instance; }
Assignment Solver::getAssignment() { return this->assignment; }
bool Solver::getResult() { return this->result; }

void Solver::solver() {
    // Convert instance to CNFFormula
    CNFFormula formula = this->instance->getFormula();
    this->result = this->solve(formula, this->assignment);
}

void assignLiteral(int literal, bool value, Assignment &cur_assignment, CNFFormula &formula) {
    cur_assignment[abs(literal)] = literal > 0 ? value : !value;
    int now_true = value ? literal : -literal;

    formula.erase(remove_if(formula.begin(), formula.end(),
                            [now_true](const Clause &c) { return c.count(now_true) > 0; }),
                  formula.end());
    for (Clause &clause : formula) {
        clause.erase(-now_true);
    }
}

bool Solver::solve(const CNFFormula &formula, Assignment &cur_assignment) {
    CNFFormula reducedFormula = formula;

    bool updated = true;
    while (updated) {
        updated = false;
        updated = updated || this->unitPropagation(reducedFormula, cur_assignment);
        updated = updated || this->pureLiteralElimination(reducedFormula, cur_assignment);
    }

    // If formula is empty, it's satisfiable
    if (reducedFormula.empty()) {
        this->assignment = cur_assignment;
        return true;
    }

    // If any empty clause exists, return false (conflict)
    for (const Clause &clause : reducedFormula) {
        if (clause.empty())
            return false;
    }

    // Choose a literal heuristically (first unassigned variable)
    int literal = chooseLiteral(reducedFormula);

    Assignment assignment_t = cur_assignment;
    CNFFormula formula_t = reducedFormula;
    assignLiteral(literal, true, assignment_t, formula_t);

    // Recur with updated formula
    if (solve(formula_t, assignment_t))
        return true;

    // Restore assignment before trying the other branch
    Assignment assignment_f = cur_assignment;
    CNFFormula formula_f = reducedFormula;
    assignLiteral(literal, false, assignment_f, formula_f);

    return solve(formula_f, assignment_f);
}

bool Solver::unitPropagation(CNFFormula &formula, Assignment &assignment) {
    bool changed = true;
    bool updated = false;
    while (changed) {
        changed = false;
        for (auto it = formula.begin(); it != formula.end();) {
            if (it->size() == 1) { // Found a unit clause
                int unit = *it->begin();
                assignLiteral(unit, true, assignment, formula);

                changed = true;
                updated = true;

                break;
            } else {
                ++it;
            }
        }
    }
    return updated;
}

bool Solver::pureLiteralElimination(CNFFormula &formula, Assignment &assignment) {
    unordered_map<int, int> literalCounts;
    bool updated = false;

    for (const Clause &clause : formula) {
        for (int lit : clause) {
            literalCounts[lit]++;
        }
    }

    for (const auto &[lit, count] : literalCounts) {
        if (literalCounts.count(-lit) == 0) { // It's a pure literal
            updated = true;
            assignLiteral(lit, true, assignment, formula);
        }
    }
    return updated;
}

int Solver::chooseLiteral(const CNFFormula &formula) {
    // Parameters for combining heuristics
    double alpha = 2.0;              // Jeroslow-Wang weight
    double beta = 1.0;               // Pure frequency weight
    double gamma = 0.5;              // MOM-like weight
    unordered_map<int, double> freq; // Count occurrences
    unordered_map<int, double> jw;   // Jeroslow-Wang scores
    unordered_map<int, double> mom;  // Weighted by smallest clauses
    // Find smallest clause size for MOM
    size_t minClauseSize = numeric_limits<size_t>::max();
    for (const Clause &c : formula) {
        if (!c.empty() && c.size() < minClauseSize) {
            minClauseSize = c.size();
        }
    }
    // Build scores
    for (const Clause &clause : formula) {
        double weight = pow(2.0, -static_cast<double>(clause.size())); // Jeroslow-Wang
        for (int lit : clause) {
            freq[lit]++;
            jw[lit] += weight;
            if (clause.size() == minClauseSize) {
                mom[lit] += 1.0; // Count occurrences in smallest clauses
            }
        }
    }
    // Combine scores
    int bestLiteral = 0;
    double bestScore = -1.0;
    for (auto &entry : freq) {
        int lit = entry.first;
        double combinedScore = alpha * jw[lit] + beta * freq[lit] + gamma * mom[lit];
        if (combinedScore > bestScore) {
            bestScore = combinedScore;
            bestLiteral = lit;
        }
    }
    return bestLiteral;
}
