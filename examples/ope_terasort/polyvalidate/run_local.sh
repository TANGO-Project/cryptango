#!/bin/bash
runcompss -d --lang=c --project=project_localhost.xml master/polyvalidate 4 secrets output/output validate/validate timings/timings result timings.csv
