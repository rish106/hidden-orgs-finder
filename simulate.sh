#!/usr/bin/env sh

n="$1"
k1="$2"
k2="$3"

filename="test"

python3 ./my_problem_gen.py $n $k1 $k2 && \
    ./compile.sh && \
    ./run1.sh "$filename" && \
    minisat "$filename.satinput" "$filename.satoutput" > "$filename.log"

./run2.sh "$filename"
