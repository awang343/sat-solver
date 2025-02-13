#ifndef DPLL_H
#define DPLL_H

#include "sat_instance.h"
class Solver {
  private:
    SATInstance* instance;

  public:
    Solver();

    void loadInstance(SATInstance &instance);
    void solveInstance();
    void unitProp();
};

#endif
