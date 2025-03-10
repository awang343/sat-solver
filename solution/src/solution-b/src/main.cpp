#include "dimacs_parser.h"
#include "dpll.h"
#include "sat_instance.h"
#include "timer.h"

#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = filesystem;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./main <cnf file>" << endl;
        return 1;
    }

    string input = argv[1];
    fs::path path(input);
    string filename = path.filename().string();

    Timer watch;
    watch.start();

    SATInstance instance = parseCNFFile(input);
    Solver solver = Solver();
    solver.setInstance(instance);
    /* cout << instance.toString() << endl; */
    bool sat = solver.solve();
    watch.stop();

    Assignment assignment = solver.getAssignment();

    if (sat) {
        string solution;
        for (const auto &[key, value] : assignment) {
            solution += to_string(key) + " " + (value == 1 ? "true" : "false") + " ";
        }
        if (!solution.empty()) {
            solution.pop_back(); // Remove the trailing space
        }

        cout << "{\"Instance\": \"" << filename << "\", \"Time\": " << fixed << setprecision(2)
             << watch.getTime() << ", \"Result\": \"SAT\", \"Solution\": \"" << solution << "\"}"
             << endl;
    } else {
        cout << "{\"Instance\": \"" << filename << "\", \"Time\": " << fixed << setprecision(2)
             << watch.getTime() << ", \"Result\": \"UNSAT\"}" << endl;
    }

    return 0;
}
