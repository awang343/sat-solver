#ifndef DPLL_H
#define DPLL_H

#include "sat_instance.h"
#include "types.h"

class Solver {
  private:
    SATInstance* instance;
    bool solve(CNFFormula formula, Assignment &assignment);
    void unitPropagation(CNFFormula &formula, Assignment &assignment);
    void pureLiteralElimination(CNFFormula &formula, Assignment &assignment);
    int chooseLiteral(const CNFFormula &formula);

  public:
    Solver();
    void loadInstance(SATInstance &instance);
    void solver();
    void unitProp();
};

#endif
