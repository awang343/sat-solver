#ifndef SAT_INSTANCE_H
#define SAT_INSTANCE_H

#include "types.h"
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class SATInstance {
  public:
    SATInstance(int numVars, int numClauses);

    void addVariable(int literal);
    void addClause(const Clause &clause);

    int getNumVars();
    int getNumClauses();

    std::unordered_set<int> getVars();
    CNFFormula getFormula();

    std::string toString() const;

  private:
    int numVars;
    int numClauses;
    std::unordered_set<int> vars;
    CNFFormula formula;
};

#endif
