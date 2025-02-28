#ifndef DPLL_H
#define DPLL_H

#include "sat_instance.h"
#include "types.h"

class Solver {
  private:
    SATInstance *instance;

    bool dpll();
    bool propagate();
    bool pureLiteralElimination();
    int chooseLiteral();

  public:
    Solver();
    void setInstance(SATInstance &instance);
    std::vector<int> getAssignment();
    bool solve();
};

#endif
