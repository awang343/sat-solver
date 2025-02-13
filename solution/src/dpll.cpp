#include "dpll.h"
#include "sat_instance.h"

using namespace std;

Solver::Solver() : instance() {}

void Solver::loadInstance(SATInstance &instance) { this->instance = &instance; }
void Solver::unitProp() {
    cout << "Variables: [ ";
    for (int v : instance->getVars()) {
        cout << v << " ";
    }
    cout << "]\n";
}
