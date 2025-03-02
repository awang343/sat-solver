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

solution_a_list=("")
solution_b_list=("")

# Update this file with instructions on how to run your code given an input
# ./bin/solution-a $input
if [[ " ${solution_a_list[@]} " =~ " ${input} " ]]; then
    ./bin/solution-a $input
# Check if input is in solution-b list
elif [[ " ${solution_b_list[@]} " =~ " ${input} " ]]; then
    ./bin/solution-b $input
fi
