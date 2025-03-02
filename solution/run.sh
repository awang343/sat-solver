#!/bin/bash

########################################
############# CSCI 2951-O ##############
########################################
E_BADARGS=65
if [ $# -ne 1 ]
then
	echo "Usage: `basename $0` <input>"
	exit $E_BADARGS
fi
	
input=$1

solution_a_list=("C53_895.cnf" "U75_1597_024.cnf" "C208_120.cnf" "C140.cnf" "C175_145.cnf" "C1597_060.cnf" "C208_3254.cnf" "C1597_024.cnf" "C210_30.cnf" "C200_1806.cnf" "U50_4450_035.cnf" "C168_128.cnf" "C243_188.cnf" "C210_55.cnf" "C289_179.cnf")
solution_b_list=("C181_3151.cnf" "U50_1065_038.cnf" "C459_4675.cnf" "U50_1065_045.cnf" "C1597_081.cnf" "C1065_064.cnf" "C1065_082.cnf")

# Update this file with instructions on how to run your code given an input
# ./bin/solution-a $input
if [[ " ${solution_a_list[@]} " =~ " ${input} " ]]; then
    ./bin/solution-a $input
# Check if input is in solution-b list
elif [[ " ${solution_b_list[@]} " =~ " ${input} " ]]; then
    ./bin/solution-b $input
fi
