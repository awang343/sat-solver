import sys
from pathlib import Path
from timer import Timer
from dimacs_parser import parse_cnf_file

def pure_literal_elimination(clauses, assignment):
    all_literals = set()
    for c in clauses:
        all_literals |= set(abs(l) for l in c)
    # track how each literal appears
    counts = {}
    for c in clauses:
        for l in c:
            counts.setdefault(l, 0)
            counts[l] += 1
    # find pure literals
    changed = True
    while changed:
        changed = False
        for lit in list(counts.keys()):
            if -lit not in counts:
                # lit is pure
                val = lit > 0
                assignment[abs(lit)] = val
                # remove clauses containing lit
                clauses = [c for c in clauses if lit not in c]
		del counts[lit]
                changed = True
                break
    return clauses, assignment

def unit_clause_elimination(clauses, assignment):
    changed = True
    while changed:
        changed = False
        for c in clauses:
            if len(c) == 1:
                lit = next(iter(c))
                val = (lit > 0)
                assignment[abs(lit)] = val
                # remove clauses containing lit
                clauses = [x for x in clauses if lit not in x]
                # remove ~lit from clauses
                new_clauses = []
                for x in clauses:
                    if -lit in x:
                        y = set(x)
                        y.discard(-lit)
                        new_clauses.append(y)
                    else:
                        new_clauses.append(x)
                clauses = new_clauses
                changed = True
                break
    return clauses, assignment

def dpll(clauses, assignment):
    clauses, assignment = pure_literal_elimination(clauses, assignment)
    clauset
    1, assignment = unit_clause_elimination(clauses, assignment)
    # check for empty clauses or success
    if any(len(c) == 0 for c in clauses):
        return False, assignment  # unsatisfiable
    if len(clauses) == 0:
        return True, assignment  # satisfied

    # pick a literal to branch on (simple heuristic: pick from first clause)
    first_clause = clauses[0]
    lit = next(iter(first_clause))

    # branch true
    new_assign = dict(assignment)
    new_assign[abs(lit)] = lit > 0
    new_clauses = []
    for c in clauses:
        if lit in c:
            continue  # satisfied
        if -lit in c:
            d = set(c)
            d.discard(-lit)
            new_clauses.append(d)
        else:
            new_clauses.append(c)
    sat, result_assign = dpll(new_clauses, new_assign)
    if sat:
        return True, result_assign
    
    # branch false
    new_assign = dict(assignment)
    new_assign[abs(lit)] = lit < 0
    new_clauses = []
    for c in clauses:
        if -lit in c:
            continue
        if lit in c:
            d = set(c)
            d.discard(lit)
            new_clauses.append(d)
        else:
            new_clauses.append(c)
    return dpll(new_clauses, new_assign)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python main.py <cnf file>")
        sys.exit(0)

    input_file = sys.argv[1]
    filename = Path(input_file).name

    watch = Timer()
    watch.start()

    inst = parse_cnf_file(input_file)
    print(inst)

    # Solve with DPLL
    sat, assignment = dpll(inst.clauses, {})
    watch.stop()
    t = watch.get_time()
    res = "SAT" if sat else "UNSAT"

    print(f'{{"Instance": "{filename}", "Time": {t:.2f}, "Result": "{res}"}}')
