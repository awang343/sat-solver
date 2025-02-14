#include "sat_instance.h"
#include "types.h"

using namespace std;

// Constructor
SATInstance::SATInstance(int numVars, int numClauses) : numVars(numVars), numClauses(numClauses) {}

// Adds a variable, storing only positive literals
void SATInstance::addVariable(int literal) { vars.insert(abs(literal)); }

int SATInstance::getNumVars() { return numVars; }
int SATInstance::getNumClauses() { return numClauses; }

std::unordered_set<int> SATInstance::getVars() { return vars; }
CNFFormula SATInstance::getFormula() { return formula; }

// Adds a clause (set of literals)
void SATInstance::addClause(const Clause &clause) { formula.push_back(clause); }

// Converts the SATInstance object to a string representation
string SATInstance::toString() const {
    ostringstream buf;
    buf << "Number of variables: " << numVars << "\n";
    buf << "Number of clauses: " << numClauses << "\n";
    buf << "Variables: [ ";
    for (int v : vars) {
        buf << v << " ";
    }
    buf << "]\n";

    for (size_t c = 0; c < formula.size(); ++c) {
        buf << "Clause " << c << ": [ ";
        for (int lit : formula[c]) {
            buf << lit << " ";
        }
        buf << "]\n";
    }
    return buf.str();
}
