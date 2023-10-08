#!/bin/bash

if [[ $# -lt 2 ]] || [[ $# -gt 3 ]]; then
    echo "Invalid arguments, enter 'N', 'K1', 'K2' or 'N', 'K'"
    exit 1
fi
n="$1"
k1="$2"
k2="$3"

filename="test"

date_cmd="gdate"
which $date_cmd > /dev/null
if [[ $(echo $?) -ne 0 ]]; then
    date_cmd="date"
fi
print_time=1

if [[ $# -eq 3 ]]; then
    problem_num=1
else
    problem_num=2
fi

./clean.sh

echo "Generating problem" && \
    python3 ./my_problem_gen.py $n $k1 $k2 && \
    echo "Compiling" && \
    ./compile.sh

exit_code=$?

script_start=$($date_cmd +%s%3N)

if [[ $(echo $script_start | grep "N") ]]; then
    print_time=0
fi

if [[ $problem_num -eq 1 ]] && [[ $exit_code -eq "0" ]]; then
    echo "Creating SAT input" && \
        ./run1.sh "$filename" && \
        echo "Running minisat" && \
        minisat "$filename.satinput" "$filename.satoutput" > "$filename.log"
    echo "Mapping vertices" && \
        ./run2.sh "$filename" && \
        cat "$filename.mapping"
elif [[ $exit_code -eq "0" ]]; then
    echo "Running minisat" && \
        ./run3.sh "$filename" && \
        cat "$filename.mapping"
fi

if [[ $print_time -eq 1 ]]; then
    script_end=$($date_cmd +%s%3N)
    elapsed_milliseconds=$((script_end - script_start))
    elapsed_seconds=$((elapsed_milliseconds / 1000))
    elapsed_milliseconds=$((elapsed_milliseconds - 1000 * elapsed_seconds))
    elapsed_milliseconds=$(printf "%03g" $elapsed_milliseconds)
    echo "Finished in ${elapsed_seconds}.${elapsed_milliseconds} seconds"
fi
