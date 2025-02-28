#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

using Assignment = std::unordered_map<int, bool>;
using Clause = std::unordered_set<int>;
using WatchedLiterals = std::unordered_map<int, std::vector<Clause *>>;
using CNFFormula = std::vector<Clause>;

#endif
