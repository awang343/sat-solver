#include "dpll.h"
#include "sat_instance.h"
#include "types.h"

using namespace std;

Solver::Solver() : instance() {}
void Solver::setInstance(SATInstance &instance) { this->instance = &instance; }
Assignment Solver::getAssignment() { return instance->assignment; }

void printAssignments(Assignment assignment) {
    for (const auto &[key, value] : assignment) {
        if (value != 0)
            cout << key << " " << value << "|";
    }
    cout << endl;
}
void printWatchers(WatchedLiterals watchers) {
    for (const auto &entry : watchers) {
        std::cout << "Key: " << entry.first << "\n";
        std::cout << "  Values: \n";

        for (const auto &pair : entry.second) {
            std::cout << "    (" << pair.first << ", " << pair.second << ")\n";
        }

        std::cout << std::endl; // Print a blank line for better readability
    }
}

/*{{{ Propagate*/
bool Solver::propagate() {
    while (!instance->propQueue.empty()) {
        // Pop the first of propQueue, this is a literal we forced to be true
        int p = instance->propQueue.front();
        instance->propQueue.pop();
        int negP = -p; // This literal is forced to be false
        // We process all clauses that are watching -p because -p is false
        /* printWatchers(instance->watchers); */
        set<pair<size_t, int>> currentWatchers = instance->watchers[negP];
        for (auto &entry : currentWatchers) {
            size_t ci = entry.first;    // Index of the clause in the clauses map
            int watchId = entry.second; // First (0) or second (1) watched literal in clause

            Clause &clause = instance->clauses[ci];

            // If there is no other watched literal, we conflict since negP is definitely false
            optional<size_t> otherWatchIdxOpt = (watchId == 0 ? clause.watch2 : clause.watch1);
            if (!otherWatchIdxOpt.has_value()) {
                return false;
            }

            size_t otherWatchIdx = *otherWatchIdxOpt;

            // If the other watched literal is already true, then the clause is sat.
            int otherLit = clause.literals[static_cast<size_t>(otherWatchIdx)];
            if (instance->isTrue(otherLit)) {
                /* clause.satisfied = true; */
                continue;
            }

            // Otherwise, try to find a new literal in the clause to watch
            bool foundNewWatch = false;
            for (size_t k = 0; k < clause.literals.size(); k++) {
                int k_int = static_cast<int>(k);
                if (k_int == clause.watch1 || k_int == clause.watch2)
                    continue;
                int candidate = clause.literals[k];

                // If a literal is unassigned or true, we can watch it
                if (!instance->isFalse(candidate)) {
                    if (watchId == 0)
                        clause.watch1 = k_int;
                    else
                        clause.watch2 = k_int;
                    instance->watchers[negP].erase(entry);
                    instance->watchers[candidate].insert(entry);
                    foundNewWatch = true;
                    break;
                }
            }

            if (!foundNewWatch) {
                // No replacement was found, so the other literal must be true
                // Otherwise, there is a conflict
                int val = (otherLit > 0) ? 1 : -1;

                if (instance->isFalse(otherLit)) {
                    instance->watchers[negP].insert(entry);
                    return false; // conflict detected
                } else if (instance->isTrue(otherLit)) {
                    /* clause.satisfied = true; */
                } else if (instance->assignment[abs(otherLit)] == 0) {
                    /* cout << "Propagation sets " << otherLit << endl; */
                    instance->assignment[abs(otherLit)] = val;
                    instance->propQueue.push(otherLit);
                }

                // Re-add this watcher (still watching -p).
                instance->watchers[negP].insert(entry);
            }
        }
        /* printWatchers(instance->watchers); */
    }
    return true;
} /*}}}*/

/*{{{ Pure Literal Elimination*/
bool Solver::pureLiteralElimination() {
    bool changed = true;
    while (changed) {
        changed = false;
        unordered_map<int, int> polarities;
        // Count appearances of each literal (only if the variable is unassigned).
        for (const auto &clause : instance->clauses) {
            if (clause.satisfied) {
                continue;
            }
            for (int lit : clause.literals) {
                if (instance->assignment[abs(lit)] != 0) {
                    continue;
                }
                if (lit > 0) {
                    if (polarities[lit] == 0)
                        polarities[lit] = 1;
                    else if (polarities[lit] == -1)
                        polarities[lit] = 2;
                } else {
                    if (polarities[-lit] == 0)
                        polarities[-lit] = -1;
                    else if (polarities[-lit] == 1)
                        polarities[-lit] = 2;
                }
            }
        }
        // For each variable that is pure, assign it.
        for (const auto &[key, value] : polarities) {
            if (value == 1) {
                instance->assignment[key] = 1;
                instance->propQueue.push(key);
                changed = true;
            } else if (value == -1) {
                instance->assignment[key] = -1;
                instance->propQueue.push(-key);
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
    unordered_map<int, double> score;
    // Assign higher weight to literals in shorter clauses
    for (const auto &clause : instance->clauses) {
        if (clause.satisfied) {
            continue; // Skip the clause if it's already sat
        }

        double weight = pow(2.0, -static_cast<double>(clause.literals.size()));
        for (int lit : clause.literals) {
            score[lit] += weight;
        }
    }
    // Pick the literal with the best score
    int bestLiteral = 0;
    double bestScore = -1.0;
    for (const auto &[lit, s] : score) {
        if (s > bestScore && instance->assignment[abs(lit)] == 0) {
            bestScore = s;
            bestLiteral = lit;
        }
    }
    return bestLiteral; // 0 if all are empty or not found
}
/*}}}*/

/*{{{ DPLL Loop*/
bool Solver::dpll() {
    // First, perform pure literal elimination and propagate any unit clauses.
    /* if (!pureLiteralElimination()) { */
    /*     return false; */
    /* } */
    if (!propagate()) {
        return false;
    }

    // Check if all variables are assigned.
    bool complete = true;
    for (int v = 1; v <= instance->getNumVars(); v++) {
        if (instance->assignment[v] == 0) {
            complete = false;
            break;
        }
    }
    if (complete) {
        return true;
    }

    // Choose a literal to branch on.
    int lit = chooseLiteral();
    if (lit == 0)
        return false; // no literal found

    // Save state for backtracking.
    /* cout << "Save point" << endl; */
    auto assignment_backup = instance->assignment;
    vector<Clause> clauses_backup = instance->clauses;
    queue<int> propQueue_backup = instance->propQueue;

    // Branch with the chosen literal set to true.
    /* cout << "Branching on literal " << lit << endl; */
    /* cout << "Trying " << lit << endl; */
    instance->assignment[abs(lit)] = lit > 0 ? 1 : -1;
    instance->propQueue.push(lit);
    if (propagate() && dpll()) {
        return true;
    }

    // Restore state
    /* cout << "Backtrack" << endl; */
    instance->assignment = assignment_backup;
    instance->clauses = clauses_backup;
    instance->propQueue = propQueue_backup;

    // Try opposite assignment
    /* cout << "Trying " << -lit << endl; */
    instance->assignment[abs(lit)] = lit < 0 ? 1 : -1;
    instance->propQueue.push(-lit);
    if (propagate() && dpll()) {
        return true;
    }

    /* cout << "Backtrack" << endl; */
    // Restore state and return failure.
    instance->assignment = assignment_backup;
    instance->clauses = clauses_backup;
    instance->propQueue = propQueue_backup;
    return false;
} /*}}}*/

/*{{{ Solve Main*/
bool Solver::solve() {
    instance->initWatchers();
    return dpll();
} /*}}}*/
