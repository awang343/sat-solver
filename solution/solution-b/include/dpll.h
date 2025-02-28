#ifndef DPLL_H
#define DPLL_H

#include "sat_instance.h"
#include "types.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unistd.h>

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
    Assignment getAssignment();
    bool solve();
};

#endif
