#!/bin/bash

if [[ $# -lt 2 ]] || [[ $# -gt 3 ]]; then
    echo "Invalid arguments, enter 'N', 'K1', 'K2' or 'N', 'K'"
    exit 1
fi
n="$1"
k1="$2"
k2="$3"

filename="test"

if [[ $# -eq 3 ]]; then
    problem_num=1
else
    problem_num=2
fi

echo "Generating problem" && \
    python3 ./my_problem_gen.py $n $k1 $k2 && \
    echo "Compiling" && \
    ./compile.sh

exit_code=$?

if [[ $problem_num -eq 1 ]] && [[ $exit_code -eq "0" ]]; then
    echo "Creating SAT input" && \
        ./run1.sh "$filename" && \
        echo "Running minisat" && \
        minisat "$filename.satinput" "$filename.satoutput" > "$filename.log"
    echo "Mapping vertices" && \
        ./run2.sh "$filename" && \
        cat "$filename".mapping
elif [[ $exit_code -eq "0" ]]; then
    echo "Running minisat" && \
        ./run3.sh "$filename" && \
        cat "$filename".mapping
fi
