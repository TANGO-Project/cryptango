#!/bin/bash
runcompss -g -d --project=project_localhost.xml master/polysort 4 4 10 input _partition mappers/mapper timings/map partitions/partition output/output timings/reduce timings.csv
