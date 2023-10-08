#!/bin/bash

if [ $# != 1 ]; then
    echo "Filename required as argument"
    exit 1
fi

filename="$1"

n=$(cat "$filename.graph" | head -n 1 | awk '{print $1}')

left=0
mid=1
right=$((n))

exec_cmd() {
    echo -n "$mid "
    ./main.out "$filename" $mid
    minisat "$filename.satinput" "$filename.satoutput" > "$filename.log"
    first_line=$(cat "$filename.satoutput" | head -n 1)
    echo $first_line
}

while [[ $left -lt $right ]]; do
    mid=$(($((left + right)) / 2))
    exec_cmd
    if [[ $(echo "$first_line" | grep "UNSAT") ]]; then
        right=$((mid))
    else
        left=$((mid + 1))
    fi
done

if [[ $(echo "$first_line" | grep "UNSAT") ]]; then
    mid=$((mid - 1))
    exec_cmd
fi

./mapping.out "$filename"
