#include "sat_instance.h"
#include "types.h"

using namespace std;

SATInstance::SATInstance(int numVars, int numClauses) : numVars(numVars), numClauses(numClauses) {
    assignment.resize(static_cast<size_t>(numVars) + 1, 0);
}

// Add a clause (each clause is a vector of ints representing literals)
void SATInstance::addClause(const vector<int> &lits) { clauses.emplace_back(lits); }

// Initialize the watchers for all clauses.
void SATInstance::initWatchers() {
    watchers.clear();
    for (size_t i = 0; i < clauses.size(); i++) {
        Clause &c = clauses[i];
        if (c.watch1 != -1) {
            int lit = c.literals[static_cast<size_t>(c.watch1)];
            watchers[lit].push_back({i, 0});
        }
        if (c.watch2 != -1) {
            int lit = c.literals[static_cast<size_t>(c.watch2)];
            watchers[lit].push_back({i, 1});
        }
    }
}

bool SATInstance::isTrue(int lit) {
    size_t var = static_cast<size_t>(abs(lit));
    if (assignment[var] == 0)
        return false;
    return (lit > 0 && assignment[var] == 1) || (lit < 0 && assignment[var] == -1);
}

bool SATInstance::isFalse(int lit) {
    size_t var = static_cast<size_t>(abs(lit));
    if (assignment[var] == 0)
        return false;
    return (lit > 0 && assignment[var] == -1) || (lit < 0 && assignment[var] == 1);
}

// Getters
int SATInstance::getNumVars() { return numVars; }
int SATInstance::getNumClauses() { return numClauses; }

// Converts the SATInstance object to a string representation
string SATInstance::toString() const {
    ostringstream buf;
    buf << "Number of variables: " << numVars << "\n";
    buf << "Number of clauses: " << numClauses << "\n";

    for (size_t c = 0; c < clauses.size(); ++c) {
        buf << "Clause " << c << ": [ ";
        for (int lit : clauses[c].literals) {
            buf << lit << " ";
        }
        buf << "]\n";
    }
    return buf.str();
}
