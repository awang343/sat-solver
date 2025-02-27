#ifndef DPLL_H
#define DPLL_H

#include "sat_instance.h"
#include "types.h"

class Solver {
  private:
    SATInstance* instance;
    Assignment assignment;
    bool result;

    bool solve(const CNFFormula &formula, Assignment &assignment);
    bool unitPropagation(CNFFormula &formula, Assignment &assignment);
    bool pureLiteralElimination(CNFFormula &formula, Assignment &assignment);

    int chooseLiteral(const CNFFormula &formula);

  public:
    Solver();
    void setInstance(SATInstance &instance);
    void solver();

    bool getResult();
    Assignment getAssignment();
};

#endif
