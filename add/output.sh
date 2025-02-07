#! /bin/sh

# output.sh - put the output of some programs in ../out/

# This script assumes that build.sh has been run.

set -e

echo "Outputting"

cd ../out/

midi      > midi.txt
printable > printable.txt

cd pq-seq/

pq-seq

echo "Done"
