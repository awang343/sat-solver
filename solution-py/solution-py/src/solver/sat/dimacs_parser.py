import os

from sat_instance import SATInstance

def parse_cnf_file(file_name):
    sat_instance = None
    try:
        with open(file_name, 'r') as f:
            lines = f.readlines()

        # skip comment lines
        idx = 0
        while idx < len(lines) and lines[idx].strip().startswith('c'):
            idx += 1

        tokens = lines[idx].split()
        if tokens[0] != 'p':
            raise ValueError("Error: DIMACS file does not have problem line")
        if tokens[1] != 'cnf':
            print("Error: DIMACS file format is not cnf")
            return sat_instance

        num_vars = int(tokens[2])
        num_clauses = int(tokens[3])
        sat_instance = SATInstance(num_vars, num_clauses)
        idx += 1

        while idx < len(lines):
            line = lines[idx].strip()
            idx += 1
            if line.startswith('c') or not line:
                continue
            clause_tokens = line.split()
            if clause_tokens[-1] != '0':
                raise ValueError(f"Error: clause line does not end with 0 {clause_tokens}")
            clause = set()
            for lit_str in clause_tokens[:-1]:
                lit = int(lit_str)
                clause.add(lit)
                sat_instance.add_variable(lit)
            sat_instance.add_clause(clause)

    except FileNotFoundError:
        raise FileNotFoundError(f"Error: DIMACS file not found {file_name}")

    return sat_instance