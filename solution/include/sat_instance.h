#ifndef SAT_INSTANCE_H
#define SAT_INSTANCE_H

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class SATInstance {
private:
  int numVars;
  int numClauses;
  std::unordered_set<int> vars;                 // Stores unique variable IDs
  std::vector<std::unordered_set<int>> clauses; // List of clauses

public:
  SATInstance(int numVars, int numClauses);

  void addVariable(int literal);
  void addClause(const std::unordered_set<int> &clause);

  int getNumVars();
  int getNumClauses();

  std::unordered_set<int> getVars();
  std::vector<std::unordered_set<int>> getClauses();

  std::string toString() const;
};

#endif
