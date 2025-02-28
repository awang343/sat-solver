#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;

// A clause is a disjunction of literals.
// Each literal is represented as an int (positive for a positive literal,
// negative for a negated variable). For watched literals, we keep two indices.
struct Clause {
    vector<int> literals;
    // Indices (into literals vector) for the two watched literals.
    // For unit clauses only watch1 is valid.
    int watch1, watch2;
    Clause(const vector<int>& lits) : literals(lits), watch1(-1), watch2(-1) {
        if (!literals.empty()) {
            watch1 = 0;
        }
        if (literals.size() > 1) {
            watch2 = 1;
        }
    }
};

class SATSolver {
public:
    // List of clauses
    vector<Clause> clauses;
    // Watchers: maps a literal (e.g. 3 or -3) to a list of pairs:
    // each pair is (clause index, which watch index in that clause: 0 for watch1, 1 for watch2)
    unordered_map<int, vector<pair<int,int>>> watchers;
    // Assignment for variables: index 1..numVars.
    // 0 means unassigned, 1 means true, -1 means false.
    vector<int> assignment;
    int numVars;
    // Propagation queue: literals that have just been assigned.
    queue<int> propQueue;

    SATSolver(int vars) : numVars(vars) {
        assignment.resize(numVars + 1, 0);
    }

    // Add a clause (each clause is a vector of ints representing literals)
    void addClause(const vector<int>& lits) {
        clauses.emplace_back(lits);
    }

    // Initialize the watchers for all clauses.
    void initWatchers() {
        watchers.clear();
        for (int i = 0; i < clauses.size(); i++) {
            Clause &c = clauses[i];
            if (c.watch1 != -1) {
                int lit = c.literals[c.watch1];
                watchers[lit].push_back({i, 0});
            }
            if (c.watch2 != -1) {
                int lit = c.literals[c.watch2];
                watchers[lit].push_back({i, 1});
            }
        }
    }

    // Helper functions for checking literal values given the current assignment.
    bool isTrue(int lit) {
        int var = abs(lit);
        if (assignment[var] == 0)
            return false;
        return (lit > 0 && assignment[var] == 1) || (lit < 0 && assignment[var] == -1);
    }

    bool isFalse(int lit) {
        int var = abs(lit);
        if (assignment[var] == 0)
            return false;
        return (lit > 0 && assignment[var] == -1) || (lit < 0 && assignment[var] == 1);
    }

    // Propagate unit clauses using watched literals.
    // This function processes all literals currently in the propagation queue.
    // Returns false if a conflict is detected.
    bool propagate() {
        while (!propQueue.empty()) {
            int p = propQueue.front();
            propQueue.pop();
            int negP = -p;
            // We process all clauses that are watching -p.
            // (Copy the current watcher list because it may be modified during iteration.)
            vector<pair<int,int>> currentWatchers = watchers[negP];
            watchers[negP].clear(); // They will be re-added as needed.
            for (auto &entry : currentWatchers) {
                int ci = entry.first;
                int watchIdx = entry.second;
                Clause &clause = clauses[ci];
                // Determine the other watched literal in the clause.
                int otherWatchIdx = (watchIdx == 0 ? clause.watch2 : clause.watch1);
                // If the clause is unit (only one watched literal exists), then otherWatchIdx might be -1.
                if (otherWatchIdx == -1) {
                    // Unit clause: if the only literal is false, conflict.
                    int sole = clause.literals[clause.watch1];
                    if (isFalse(sole))
                        return false;
                    // Otherwise, nothing to do.
                    watchers[negP].push_back(entry);
                    continue;
                }
                int otherLit = clause.literals[otherWatchIdx];
                // If the other watched literal is already true, then the clause is satisfied.
                if (isTrue(otherLit)) {
                    watchers[negP].push_back(entry);
                    continue;
                }
                // Try to find a new literal in the clause to watch (other than the two already watched).
                bool foundNewWatch = false;
                for (int k = 0; k < clause.literals.size(); k++) {
                    if (k == clause.watch1 || k == clause.watch2)
                        continue;
                    int candidate = clause.literals[k];
                    if (!isFalse(candidate)) { // candidate is unassigned or true
                        // Update the watched literal.
                        if (watchIdx == 0)
                            clause.watch1 = k;
                        else
                            clause.watch2 = k;
                        watchers[candidate].push_back({ci, watchIdx});
                        foundNewWatch = true;
                        break;
                    }
                }
                if (!foundNewWatch) {
                    // No replacement was found.
                    // Then the clause is either unit (if otherLit is unassigned) or a conflict.
                    if (isFalse(otherLit))
                        return false;  // conflict detected
                    // Unit clause: assign otherLit.
                    int var = abs(otherLit);
                    int val = (otherLit > 0) ? 1 : -1;
                    if (assignment[var] == 0) {
                        assignment[var] = val;
                        propQueue.push(otherLit);
                    }
                    // Re-add this watcher (still watching -p).
                    watchers[negP].push_back(entry);
                }
            }
        }
        return true;
    }

    // Perform pure literal elimination.
    // A pure literal appears with only one polarity in all clauses.
    // If found, it can be set to satisfy all clauses containing it.
    bool pureLiteralElimination() {
        bool changed = true;
        while (changed) {
            changed = false;
            vector<int> posCount(numVars + 1, 0);
            vector<int> negCount(numVars + 1, 0);
            // Count appearances of each literal (only if the variable is unassigned).
            for (const auto &clause : clauses) {
                for (int lit : clause.literals) {
                    if (assignment[abs(lit)] != 0)
                        continue;
                    if (lit > 0)
                        posCount[abs(lit)]++;
                    else
                        negCount[abs(lit)]++;
                }
            }
            // For each variable that is pure, assign it.
            for (int v = 1; v <= numVars; v++) {
                if (assignment[v] != 0)
                    continue;
                if (posCount[v] > 0 && negCount[v] == 0) {
                    assignment[v] = 1;
                    propQueue.push(v);
                    changed = true;
                } else if (negCount[v] > 0 && posCount[v] == 0) {
                    assignment[v] = -1;
                    propQueue.push(-v);
                    changed = true;
                }
            }
            if (changed && !propagate())
                return false;
        }
        return true;
    }

    // A simple heuristic for choosing a literal to branch on.
    // This heuristic counts the occurrence of each literal in the clauses and
    // picks the unassigned variable with the highest total count.
    // It returns a literal with a polarity corresponding to the majority appearance.
    int chooseLiteral() {
        int bestVar = 0;
        int bestScore = -1;
        int bestPolarity = 1;
        vector<int> posCount(numVars + 1, 0);
        vector<int> negCount(numVars + 1, 0);
        for (const auto &clause : clauses) {
            for (int lit : clause.literals) {
                if (assignment[abs(lit)] != 0)
                    continue;
                if (lit > 0)
                    posCount[abs(lit)]++;
                else
                    negCount[abs(lit)]++;
            }
        }
        for (int v = 1; v <= numVars; v++) {
            if (assignment[v] == 0) {
                int score = posCount[v] + negCount[v];
                if (score > bestScore) {
                    bestScore = score;
                    bestVar = v;
                    bestPolarity = (posCount[v] >= negCount[v]) ? 1 : -1;
                }
            }
        }
        // If no unassigned variable is found, return 0 (should not happen)
        return bestVar == 0 ? 0 : bestVar * bestPolarity;
    }

    // The main DPLL recursive function.
    // Returns true if a satisfying assignment is found.
    bool dpll() {
        // First, perform pure literal elimination and propagate any unit clauses.
        if (!pureLiteralElimination())
            return false;
        if (!propagate())
            return false;
        // Check if all variables are assigned.
        bool complete = true;
        for (int v = 1; v <= numVars; v++) {
            if (assignment[v] == 0) {
                complete = false;
                break;
            }
        }
        if (complete)
            return true;

        // Choose a literal to branch on.
        int lit = chooseLiteral();
        if (lit == 0)
            return false;  // no literal found

        // Save state for backtracking.
        vector<int> assignment_backup = assignment;
        auto watchers_backup = watchers;
        vector<Clause> clauses_backup = clauses;
        queue<int> propQueue_backup = propQueue;

        // Branch with the chosen literal set to true.
        assignment[abs(lit)] = (lit > 0 ? 1 : -1);
        propQueue.push(lit);
        if (propagate() && dpll())
            return true;

        // Restore state and try the opposite assignment.
        assignment = assignment_backup;
        watchers = watchers_backup;
        clauses = clauses_backup;
        propQueue = propQueue_backup;
        assignment[abs(lit)] = (lit > 0 ? -1 : 1);
        propQueue.push(-lit);
        if (propagate() && dpll())
            return true;

        // Restore state and return failure.
        assignment = assignment_backup;
        watchers = watchers_backup;
        clauses = clauses_backup;
        propQueue = propQueue_backup;
        return false;
    }

    // Solve the SAT instance.
    bool solve() {
        initWatchers();
        return dpll();
    }

    // Print the satisfying assignment.
    void printAssignment() {
        cout << "Satisfying assignment:" << endl;
        for (int v = 1; v <= numVars; v++) {
            cout << "Variable " << v << " = " 
                 << (assignment[v] == 1 ? "true" : (assignment[v] == -1 ? "false" : "unassigned")) 
                 << endl;
        }
    }
};

int main() {
    // Example: solve (x1 OR x2) AND (¬x1 OR x3) AND (¬x2 OR ¬x3) AND (x2 OR x3)
    // Represent variables as 1, 2, 3.
    // Literals: 1 means x1, -1 means ¬x1, etc.
    int numVars = 3;
    SATSolver solver(numVars);

    solver.addClause({1, 2});      // x1 ∨ x2
    solver.addClause({-1, 3});     // ¬x1 ∨ x3
    solver.addClause({-2, -3});    // ¬x2 ∨ ¬x3
    solver.addClause({2, 3});      // x2 ∨ x3

    if (solver.solve()) {
        cout << "SATISFIABLE" << endl;
        solver.printAssignment();
    } else {
        cout << "UNSATISFIABLE" << endl;
    }
    return 0;
}
