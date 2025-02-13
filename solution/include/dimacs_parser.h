#ifndef DIMACS_PARSER_H
#define DIMACS_PARSER_H

#include "sat_instance.h"
SATInstance parseCNFFile(const std::string& fileName);

#endif
