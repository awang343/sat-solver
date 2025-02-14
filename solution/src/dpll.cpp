#include "dpll.h"
#include "sat_instance.h"
#include "types.h"
#include <algorithm>
#include <iostream>

using namespace std;

Solver::Solver() : instance() {}

void Solver::loadInstance(SATInstance &instance) { this->instance = &instance; }

void Solver::solver() {
    // Convert instance to CNFFormula
    CNFFormula formula = this->instance->getFormula();
    this->solve(formula)

    for (int literal : formula[0]) {
        cout << literal << endl;
    }
}

bool Solver::solve(CNFFormula formula, Assignment &assignment) {
    this->unitPropagation(formula, assignment);
    this->pureLiteralElimination(formula, assignment);

    // If formula is empty, it's satisfiable
    if (formula.empty())
        return true;

    // If any empty clause exists, return false (conflict)
    for (const auto &clause : formula) {
        if (clause.empty())
            return false;
    }

    // Choose a literal heuristically (first unassigned variable)
    int literal = chooseLiteral(formula);

    // Try setting it to true
    assignment[literal] = true;
    CNFFormula reducedFormula = formula;
    reducedFormula.erase(remove_if(reducedFormula.begin(), reducedFormula.end(),
                                   [literal](const Clause &c) { return c.count(literal) > 0; }),
                         reducedFormula.end());

    for (auto &clause : reducedFormula) {
        clause.erase(-literal);
    }

    if (solve(reducedFormula, assignment))
        return true;

    // If setting it to true fails, try false
    assignment[literal] = false;
    reducedFormula = formula;
    reducedFormula.erase(remove_if(reducedFormula.begin(), reducedFormula.end(),
                                   [literal](const Clause &c) { return c.count(-literal) > 0; }),
                         reducedFormula.end());

    for (auto &clause : reducedFormula) {
        clause.erase(literal);
    }

    return solve(reducedFormula, assignment);
}

void Solver::unitPropagation(CNFFormula &formula, Assignment &assignment) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto it = formula.begin(); it != formula.end();) {
            if (it->size() == 1) { // Found a unit clause
                int unit = *it->begin();
                assignment[unit] = (unit > 0);
                formula.erase(it);
                changed = true;

                // Remove clauses containing unit
                formula.erase(remove_if(formula.begin(), formula.end(),
                                        [unit](const Clause &c) { return c.count(unit) > 0; }),
                              formula.end());

                // Remove negations of unit
                for (auto &clause : formula) {
                    clause.erase(-unit);
                }
                break;
            } else {
                ++it;
            }
        }
    }
}

void Solver::pureLiteralElimination(CNFFormula &formula, Assignment &assignment) {
    unordered_map<int, int> literalCounts;

    for (const auto &clause : formula) {
        for (int lit : clause) {
            literalCounts[lit]++;
        }
    }

    for (const auto &[lit, count] : literalCounts) {
        if (literalCounts.count(-lit) == 0) { // It's a pure literal
            assignment[lit] = (lit > 0);
            formula.erase(remove_if(formula.begin(), formula.end(),
                                    [lit](const Clause &c) { return c.count(lit) > 0; }),
                          formula.end());
        }
    }
}

int Solver::chooseLiteral(const CNFFormula &formula) {
    for (const auto &clause : formula) {
        if (!clause.empty()) {
            return *clause.begin(); // Select first available literal
        }
    }
    return 0; // Should not happen
}
