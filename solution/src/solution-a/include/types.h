#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

using Assignment = std::unordered_map<int, bool>;
using Clause = std::unordered_set<int>;
using CNFFormula = std::vector<Clause>;

#endif
