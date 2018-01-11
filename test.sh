#!/bin/sh
set -eu
: $1

INPUT="dat/dat$1_in"
OUTPUT="dat/dat$1_test"
ORG="dat/dat$1_org"

make
./grpwk $INPUT $OUTPUT
if [ $2 = "-ed" ]; then
  python analysis.py $OUTPUT $ORG
fi
