#!/bin/sh
set -eu
: $1

INPUT="../dat/dat$1_in"
OUTPUT="../dat/dat$1_test"
ORG="../dat/dat$1_org"

make
./grpwk $INPUT $OUTPUT
python analysis.py $OUTPUT $ORG
