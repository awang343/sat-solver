#include "dpll.h"
#include "sat_instance.h"
#include "types.h"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

Solver::Solver() : instance() {}
void Solver::setInstance(SATInstance &instance) { this->instance = &instance; }
vector<int> Solver::getAssignment() { return instance->assignment; }

/*{{{ Propagate*/
bool Solver::propagate() {
    while (!instance->propQueue.empty()) {
        int p = instance->propQueue.front();
        instance->propQueue.pop();
        int negP = -p;
        // We process all clauses that are watching -p.
        // (Copy the current watcher list because it may be modified during iteration.)
        vector<pair<int, int>> currentWatchers = instance->watchers[negP];
        instance->watchers[negP].clear(); // They will be re-added as needed.
        for (auto &entry : currentWatchers) {
            int ci = entry.first;
	    size_t ci_idx = static_cast<size_t>(ci);

            int watchIdx = entry.second;
            Clause &clause = instance->clauses[ci_idx];

            // Determine the other watched literal in the clause.
            int otherWatchInt = (watchIdx == 0 ? clause.watch2 : clause.watch1);
	    size_t otherWatchIdx = static_cast<size_t>(otherWatchInt);
            // If the clause is unit (only one watched literal exists), then otherWatchIdx might be
            // -1.
            if (otherWatchInt == -1) {
                // Unit clause: if the only literal is false, conflict.
		size_t watch_idx = static_cast<size_t>(clause.watch1);
                int sole = clause.literals[watch_idx];
                if (instance->isFalse(sole))
                    return false;
                // Otherwise, nothing to do.
                instance->watchers[negP].push_back(entry);
                continue;
            }
            int otherLit = clause.literals[otherWatchIdx];
            // If the other watched literal is already true, then the clause is satisfied.
            if (instance->isTrue(otherLit)) {
                instance->watchers[negP].push_back(entry);
                continue;
            }
            // Try to find a new literal in the clause to watch (other than the two already
            // watched).
            bool foundNewWatch = false;
            for (size_t k = 0; k < clause.literals.size(); k++) {
		int k_int = static_cast<int>(k);
                if (k_int == clause.watch1 || k_int == clause.watch2)
                    continue;
                int candidate = clause.literals[k];
                if (!instance->isFalse(candidate)) { // candidate is unassigned or true
                    // Update the watched literal.
                    if (watchIdx == 0)
                        clause.watch1 = k_int;
                    else
                        clause.watch2 = k_int;
                    instance->watchers[candidate].push_back({ci, watchIdx});
                    foundNewWatch = true;
                    break;
                }
            }
            if (!foundNewWatch) {
                // No replacement was found.
                // Then the clause is either unit (if otherLit is unassigned) or a conflict.
                if (instance->isFalse(otherLit))
                    return false; // conflict detected
                // Unit clause: assign otherLit.
                size_t var = static_cast<size_t>(abs(otherLit));
                int val = (otherLit > 0) ? 1 : -1;
                if (instance->assignment[var] == 0) {
                    instance->assignment[var] = val;
                    instance->propQueue.push(otherLit);
                }
                // Re-add this watcher (still watching -p).
                instance->watchers[negP].push_back(entry);
            }
        }
    }
    return true;
} /*}}}*/

/*{{{ Pure Literal Elimination*/
bool Solver::pureLiteralElimination() {
    bool changed = true;
    while (changed) {
        changed = false;
	size_t size = static_cast<size_t>(instance->getNumVars());
        vector<int> posCount(size + 1, 0);
        vector<int> negCount(size + 1, 0);
        // Count appearances of each literal (only if the variable is unassigned).
        for (const auto &clause : instance->clauses) {
            for (int lit : clause.literals) {
		size_t var = static_cast<size_t>(abs(lit));
                if (instance->assignment[var] != 0)
                    continue;
                if (lit > 0)
                    posCount[var]++;
                else
                    negCount[var]++;
            }
        }
        // For each variable that is pure, assign it.
        for (int v = 1; v <= instance->getNumVars(); v++) {
	    size_t var = static_cast<size_t>(v);
            if (instance->assignment[var] != 0)
                continue;
            if (posCount[var] > 0 && negCount[var] == 0) {
                instance->assignment[var] = 1;
                instance->propQueue.push(v);
                changed = true;
            } else if (negCount[var] > 0 && posCount[var] == 0) {
                instance->assignment[var] = -1;
                instance->propQueue.push(-v);
                changed = true;
            }
        }
        if (changed && !propagate())
            return false;
    }
    return true;
} /*}}}*/

/*{{{ Choose Literal */
int Solver::chooseLiteral() {
    int bestVar = 0;
    int bestScore = -1;
    int bestPolarity = 1;
    vector<int> posCount(static_cast<size_t>(instance->getNumVars()) + 1, 0);
    vector<int> negCount(static_cast<size_t>(instance->getNumVars()) + 1, 0);

    for (const auto &clause : instance->clauses) {
        for (int lit : clause.literals) {
	    size_t idx = static_cast<size_t>(abs(lit));
            if (instance->assignment[idx] != 0)
                continue;
            if (lit > 0)
                posCount[idx]++;
            else
                negCount[idx]++;
        }
    }
    for (int v = 1; v <= instance->getNumVars(); v++) {
	size_t var = static_cast<size_t>(v);
        if (instance->assignment[var] == 0) {
            int score = posCount[var] + negCount[var];
            if (score > bestScore) {
                bestScore = score;
                bestVar = v;
                bestPolarity = (posCount[var] >= negCount[var]) ? 1 : -1;
            }
        }
    }
    // If no unassigned variable is found, return 0 (should not happen)
    return bestVar == 0 ? 0 : bestVar * bestPolarity;
} /*}}}*/

/*{{{ DPLL Loop*/
bool Solver::dpll() {
    // First, perform pure literal elimination and propagate any unit clauses.
    if (!pureLiteralElimination())
        return false;
    if (!propagate())
        return false;
    // Check if all variables are assigned.
    bool complete = true;
    for (size_t v = 1; v <= static_cast<size_t>(instance->getNumVars()); v++) {
        if (instance->assignment[v] == 0) {
            complete = false;
            break;
        }
    }
    if (complete)
        return true;

    // Choose a literal to branch on.
    int lit = chooseLiteral();
    if (lit == 0)
        return false; // no literal found
    size_t idx = static_cast<size_t>(abs(lit));

    // Save state for backtracking.
    vector<int> assignment_backup = instance->assignment;
    auto watchers_backup = instance->watchers;
    vector<Clause> clauses_backup = instance->clauses;
    queue<int> propQueue_backup = instance->propQueue;

    // Branch with the chosen literal set to true.
    instance->assignment[idx] = (lit > 0 ? 1 : -1);
    instance->propQueue.push(lit);
    if (propagate() && dpll())
        return true;

    // Restore state
    instance->assignment = assignment_backup;
    instance->watchers = watchers_backup;
    instance->clauses = clauses_backup;
    instance->propQueue = propQueue_backup;

    // Try opposite assignment
    instance->assignment[idx] = (lit > 0 ? -1 : 1);
    instance->propQueue.push(-lit);
    if (propagate() && dpll())
        return true;

    // Restore state and return failure.
    instance->assignment = assignment_backup;
    instance->watchers = watchers_backup;
    instance->clauses = clauses_backup;
    instance->propQueue = propQueue_backup;
    return false;
} /*}}}*/

/*{{{ Solve Main*/
bool Solver::solve() {
    instance->initWatchers();
    return dpll();
} /*}}}*/
