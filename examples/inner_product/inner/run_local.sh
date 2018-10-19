#!/bin/bash
# Define usage function
usage(){
	echo "Usage: $0 <num input files> <degree of polynomial to compute> <bit length of plaintexts> <effective bit length of plaintexts when enciphered>"
	exit 1
}
# Call usage() function if parameters not supplied
[[ $# -lt 4 ]] && usage

INPUT_DIR=../tango-innergen

if [  ! -d "results_$2_$3_$4" ]; then
	mkdir results_$2_$3_$4
fi
runcompss -d --lang=c --project=project_localhost.xml --resources=resources.xml master/inner $1 $2 $INPUT_DIR/parameters_$2_$3_$4 $INPUT_DIR/input_$2_$3_$4/input results_$2_$3_$4/result output_$2_$3_$4 timings_$2_$3_$4.csv
