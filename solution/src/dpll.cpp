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

void assignLiteral(int literal, bool value, Assignment &assignment) {
    if (literal > 0) {
        assignment[literal] = value;

    } else {
        assignment[-literal] = !value;
    }
}

bool Solver::solve(const CNFFormula &formula, Assignment &assignment) {
    CNFFormula reducedFormula = formula;

    bool updated = true;
    while (updated) {
        updated = false;
        updated = updated || this->unitPropagation(reducedFormula, assignment);
        updated = updated || this->pureLiteralElimination(reducedFormula, assignment);
    }

    // If formula is empty, it's satisfiable
    if (reducedFormula.empty()) {
        this->assignment = assignment;
        return true;
    }

    // If any empty clause exists, return false (conflict)
    for (const Clause &clause : reducedFormula) {
        if (clause.empty())
            return false;
    }

    // Choose a literal heuristically (first unassigned variable)
    int literal = chooseLiteral(reducedFormula);

    // Save original assignment state in case of backtracking
    Assignment originalAssignment = assignment;

    // Try setting it to true
    assignLiteral(literal, true, assignment);
    CNFFormula newFormula = reducedFormula;

    // Remove satisfied clauses and unsatisfied literals and simplify
    newFormula.erase(remove_if(newFormula.begin(), newFormula.end(),
                               [literal](const Clause &c) { return c.count(literal) > 0; }),
                     newFormula.end());
    for (Clause &clause : newFormula) {
        clause.erase(-literal);
    }

    // Recur with updated formula
    if (solve(newFormula, assignment))
        return true;

    // Restore assignment before trying the other branch
    assignment = originalAssignment;
    assignLiteral(literal, false, assignment);
    newFormula = reducedFormula;

    newFormula.erase(remove_if(newFormula.begin(), newFormula.end(),
                               [literal](const Clause &c) { return c.count(-literal) > 0; }),
                     newFormula.end());
    for (Clause &clause : newFormula) {
        clause.erase(literal);
    }

    return solve(newFormula, assignment);
}

bool Solver::unitPropagation(CNFFormula &formula, Assignment &assignment) {
    bool changed = true;
    bool updated = false;
    while (changed) {
        changed = false;
        for (auto it = formula.begin(); it != formula.end();) {
            if (it->size() == 1) { // Found a unit clause
                int unit = *it->begin();
                assignLiteral(unit, (unit > 0), assignment);

                formula.erase(it);
                changed = true;
                updated = true;

                // Remove clauses containing unit
                formula.erase(remove_if(formula.begin(), formula.end(),
                                        [unit](const Clause &c) { return c.count(unit) > 0; }),
                              formula.end());

                // Remove negations of unit
                for (Clause &clause : formula) {
                    clause.erase(-unit);
                }
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
            assignLiteral(lit, (lit > 0), assignment);
            formula.erase(remove_if(formula.begin(), formula.end(),
                                    [lit](const Clause &c) { return c.count(lit) > 0; }),
                          formula.end());
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

/* int Solver::chooseLiteral(const CNFFormula &formula) { */
/*     unordered_map<int, double> score; */
/*     // Assign higher weight to literals in shorter clauses */
/*     for (const auto &clause : formula) { */
/*         double weight = pow(2.0, -static_cast<double>(clause.size())); */
/*         for (int lit : clause) { */
/*             score[lit] += weight; */
/*         } */
/*     } */
/*     // Pick the literal with the best score */
/*     int bestLiteral = 0; */
/*     double bestScore = -1.0; */
/*     for (const auto &[lit, s] : score) { */
/*         if (s > bestScore) { */
/*             bestScore = s; */
/*             bestLiteral = lit; */
/*         } */
/*     } */
/*     return bestLiteral; // 0 if all are empty or not found */
/* } */
