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
  solver.loadInstance(instance);
  solver.solver();

  string result = "SAT";

  watch.stop();
  cout << "{\"Instance\": \"" << filename << "\", \"Time\": " << watch.getTime()
       << ", \"Result\": \"" << result << "\"}" << endl;

  return 0;
}
