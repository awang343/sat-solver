#ifndef SAT_INSTANCE_H
#define SAT_INSTANCE_H

#include "types.h"
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class SATInstance {
  public:
    SATInstance(int numVars, int numClauses);
    void addClause(const std::vector<int> &lits);
    void initWatchers();

    bool isTrue(int lit);
    bool isFalse(int lit);
    int getNumVars();
    int getNumClauses();

    std::string toString() const;

    std::vector<Clause> clauses;
    Assignment assignment;
    std::queue<int> propQueue;
    WatchedLiterals watchers;

  private:
    int numVars;
    int numClauses;
};

#endif
