#include "dpll.h"
#include "sat_instance.h"
#include "types.h"
#include <algorithm>
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
    for (const Clause &clause : formula) {
        if (!clause.empty()) {
            return *clause.begin(); // Select first available literal
        }
    }
    return 0; // Should not happen
}
