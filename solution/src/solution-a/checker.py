import re

def parse_dimacs(dimacs_file):
    clauses = []
    with open(dimacs_file, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('c') or line.startswith('p'):
                continue
            clause = [int(x) for x in line.split() if x != '0']
            if clause:
                clauses.append(clause)
    return clauses

def parse_solution(solution_str):
    assignments = {}
    tokens = solution_str.split()
    for i in range(0, len(tokens), 2):
        var = int(tokens[i])
        value = tokens[i + 1] == 'true'
        assignments[var] = value
    return assignments

def check_satisfaction(dimacs_file, solution_str):
    clauses = parse_dimacs(dimacs_file)
    assignments = parse_solution(solution_str)
    
    for clause in clauses:
        satisfied = False
        for literal in clause:
            var = abs(literal)
            if var in assignments:
                if (literal > 0 and assignments[var]) or (literal < 0 and not assignments[var]):
                    satisfied = True
                    break

        if not satisfied:
            print(clause)
            print([assignments[abs(literal)] for literal in clause])
            return False
    return True

# Example Usage
data = {"Instance": "C181_3151.cnf", "Time": 95.48, "Result": "SAT", "Solution": "173 true 176 false 164 false 167 true 180 false 158 false 155 false 162 true 181 false 172 false 163 false 153 false 149 false 146 false 154 true 177 false 168 false 150 false 108 false 104 false 101 false 105 true 97 true 92 false 94 false 72 true 74 false 4 true 2 false 170 false 161 false 152 false 107 false 145 false 144 false 141 false 137 false 143 true 91 false 1 false 51 true 52 false 178 false 169 false 160 false 142 false 106 false 118 false 117 false 116 false 114 false 113 false 115 true 53 false 171 false 44 false 159 false 32 false 100 false 90 false 80 false 30 false 157 false 89 false 29 false 156 false 19 true 79 false 20 false 147 false 67 false 66 false 65 false 64 false 62 false 49 false 129 false 70 true 11 false 138 false 63 false 9 false 136 false 68 false 39 false 166 false 22 false 81 false 56 false 85 false 16 false 75 false 17 false 46 false 73 false 14 false 37 false 83 false 58 false 87 false 13 false 140 false 130 true 3 false 71 false 12 false 139 false 60 false 50 false 36 false 128 false 10 false 69 false 99 false 40 false 34 false 84 false 179 false 61 false 15 false 82 false 59 false 18 false 131 false 132 false 5 false 133 false 6 false 134 false 7 false 135 false 8 false 103 false 112 false 121 false 148 false 21 false 175 false 48 true 26 false 57 false 77 false 41 false 42 false 43 false 45 false 47 false 174 false 28 false 38 false 165 false 78 false 88 false 86 true 98 false 76 false 96 false 31 false 33 false 35 true 25 false 55 false 95 false 93 false 109 false 151 false 24 false 54 false 110 false 120 true 119 false 122 false 123 false 124 false 125 false 126 false 127 false 102 false 111 false 27 false 23 true"}

dimacs_file = "../../inputs/" + data["Instance"]
solution_str = data["Solution"]

if check_satisfaction(dimacs_file, solution_str):
    print("Solution satisfies the DIMACS problem.")
else:
    print("Solution does NOT satisfy the DIMACS problem.")
