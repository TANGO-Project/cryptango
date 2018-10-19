#!/bin/bash
# Define usage function
usage(){
	echo "Usage: $0 <num inputs> <degree of polynomial to compute> <bit length of plaintexts> <effective bit length of plaintexts when enciphered> <number of output files to generate>"
	exit 1
}
# Call usage() function if parameters not supplied
[[ $# -lt 4 ]] && usage

if [  ! -d "input_$2_$3_$4" ]; then
	mkdir input_$2_$3_$4
fi
runcompss -d --lang=c --project=project_localhost.xml master/innergen $1 $2 $3 $4 $5 secrets_$2_$3_$4 parameters_$2_$3_$4 input_$2_$3_$4/input result_$2_$3_$4 timings_$2_$3_$4.csv

