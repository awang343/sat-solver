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
    for (const auto &clause : reducedFormula) {
        cout << "Clause" << " ";
        for (const auto &lit : clause) {
            cout << lit << " ";
        }
        cout << endl;
    }

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
                assignLiteral(unit, (unit > 0), assignment, formula);

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
            assignLiteral(lit, (lit > 0), assignment, formula);
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
