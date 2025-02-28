#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Clause {
    std::vector<int> literals;
    // Indices (into literals vector) for the two watched literals.
    // For unit clauses only watch1 is valid.
    int watch1, watch2;
    Clause(const std::vector<int> &lits) : literals(lits), watch1(-1), watch2(-1) {
        if (!literals.empty()) {
            watch1 = 0;
        }
        if (literals.size() > 1) {
            watch2 = 1;
        }
    }
};

// WatchedLiterals: maps a literal (e.g. 3 or -3) to a list of pairs:
// each pair is (clause index, which watch index in that clause: 0 for watch1, 1 for watch2)
using WatchedLiterals = std::unordered_map<int, std::vector<std::pair<int, int>>>;

#endif
