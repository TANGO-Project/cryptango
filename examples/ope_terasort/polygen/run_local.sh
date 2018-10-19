#!/bin/bash
runcompss -d --lang=c --project=project_localhost.xml master/polygen 80 32 5 10 4 10000 secrets input/input timings/timings timings.csv

