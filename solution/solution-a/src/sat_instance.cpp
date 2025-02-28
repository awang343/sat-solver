#include "sat_instance.h"
#include "types.h"

using namespace std;

SATInstance::SATInstance(int numVars, int numClauses) : numVars(numVars), numClauses(numClauses) {}

// Add a clause (each clause is a vector of ints representing literals)
void SATInstance::addClause(const vector<int> &lits) { clauses.emplace_back(lits); }

// Initialize the watchers for all clauses.
void SATInstance::initWatchers() {
    watchers.clear();
    for (size_t i = 0; i < clauses.size(); ++i) {
        // If watch1/watch2 is defined, then link it up with watchers
        Clause &c = clauses[i];
        if (c.watch1.has_value()) {
            int lit = clauses[i].literals[*clauses[i].watch1];
            watchers[lit].insert({i, 0});
        }
        if (c.watch2.has_value()) {
            int lit = clauses[i].literals[*clauses[i].watch2];
            watchers[lit].insert({i, 1});
        }
    }
}

bool SATInstance::isTrue(int lit) {
    if (assignment[abs(lit)] == 0)
        return false;
    return (lit > 0 && assignment[abs(lit)] == 1) || (lit < 0 && assignment[abs(lit)] == -1);
}

bool SATInstance::isFalse(int lit) {
    if (assignment[abs(lit)] == 0)
        return false;
    return (lit > 0 && assignment[abs(lit)] == -1) || (lit < 0 && assignment[abs(lit)] == 1);
}

// Getters
int SATInstance::getNumVars() { return numVars; }
int SATInstance::getNumClauses() { return numClauses; }

// Converts the SATInstance object to a string representation
string SATInstance::toString() const {
    ostringstream buf;
    buf << "Number of variables: " << numVars << "\n";
    buf << "Number of clauses: " << numClauses << "\n";

    std::vector<int> nums = {10, 20, 30, 40};

    for (size_t i = 0; i < clauses.size(); ++i) {
        buf << "Clause " << i << ": [ ";
        for (int lit : clauses[i].literals) {
            buf << lit << " ";
        }
        buf << "]\n";
    }
    return buf.str();
}
