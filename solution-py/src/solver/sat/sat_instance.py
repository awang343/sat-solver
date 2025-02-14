class SATInstance:
    def __init__(self, num_vars, num_clauses):
        self.num_vars = num_vars
        self.num_clauses = num_clauses
        self.vars = set()
        self.clauses = []  # each clause is a set of integer literals

    def add_variable(self, literal):
        self.vars.add(abs(literal))

    def add_clause(self, clause):
        self.clauses.append(clause)

    def __str__(self):
        buf = []
        buf.append(f"Number of variables: {self.num_vars}")
        buf.append(f"Number of clauses: {self.num_clauses}")
        buf.append(f"Variables: {self.vars}")
        for i, c in enumerate(self.clauses):
            buf.append(f"Clause {i}: {c}")
        return "\n".join(buf)