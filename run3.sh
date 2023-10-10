#!/usr/bin/env bash

if [ $# != 1 ]; then
    echo "Filename required as argument"
    exit 1
fi

filename="$1"

n=$(($(head -n 1 "$filename.graph" | awk '{print $1}')))

left=0
mid=1
right=$((n + 1))

exec_cmd() {
    ./main.out "$filename" $mid
    ./minisat "$filename.satinput" "$filename.satoutput" > /dev/null
    first_line=$(head -n 1 "$filename.satoutput")
}

while [[ $left -lt $right ]]; do
    mid=$(($((left + right)) / 2))
    exec_cmd
    if [[ -z $(echo "$first_line" | grep "^SAT") ]]; then
        right=$((mid))
    else
        left=$((mid + 1))
    fi
done

if [[ -z $(echo "$first_line" | grep "^SAT") ]]; then
    mid=$((mid - 1))
    exec_cmd
fi

./mapping.out "$filename" 2
