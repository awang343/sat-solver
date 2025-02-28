#include "dimacs_parser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

using namespace std;

SATInstance parseCNFFile(const string &fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Error: DIMACS file not found: " + fileName);
    }

    string line;
    vector<string> tokens;

    // Skip comment lines (lines starting with 'c')
    while (getline(file, line)) {
        istringstream iss(line);
        tokens.clear();
        string token;
        while (iss >> token)
            tokens.push_back(token);

        if (!tokens.empty() && tokens[0] != "c") {
            break;
        }
    }

    // Ensure first non-comment line is the problem line
    if (tokens.empty() || tokens[0] != "p") {
        throw invalid_argument("Error: DIMACS file does not have a valid problem line.");
    }

    if (tokens.size() < 4 || tokens[1] != "cnf") {
        throw runtime_error("Error: DIMACS file format is not CNF.");
    }

    int numVars = stoi(tokens[2]);
    int numClauses = stoi(tokens[3]);
    SATInstance satInstance(numVars, numClauses);

    // Parse clauses
    vector<int> clause;
    while (getline(file, line)) {
        istringstream iss(line);
        tokens.clear();
        string token;
        while (iss >> token)
            tokens.push_back(token);

        if (tokens.empty() || tokens[0] == "c") {
            continue; // Skip comments
        }

        if (tokens.back() != "0") {
            throw invalid_argument("Error: Clause line does not end with 0.");
        }

        // Process literals in clause
        for (size_t i = 0; i < tokens.size() - 1; ++i) {
            if (tokens[i].empty())
                continue;

            int literal = stoi(tokens[i]);
            clause.push_back(literal);
        }

        // Add clause to SATInstance
        satInstance.addClause(clause);
        clause.clear();
    }

    return satInstance;
}
