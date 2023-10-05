#!/bin/bash

if [ $# != 1 ]; then
    echo "Filename required as argument"
    exit 1
fi

date_cmd="gdate"
which $date_cmd > /dev/null
if [[ $(echo $?) -ne 0 ]]; then
    date_cmd="date"
fi

script_start=$($date_cmd +%s%3N)

filename="$1"

n=$(cat "$filename".graph | head -n 1 | awk '{print $1}')

left=0
mid=1
right=$((n))

exec_cmd() {
    ./main.out "$filename" $mid
    start_time=$($date_cmd +%s%3N)
    minisat "$filename".satinput "$filename".satoutput > /dev/null
    end_time=$($date_cmd +%s%3N)
    elapsed_milliseconds=$((end_time - start_time))
    elapsed_seconds=$((elapsed_milliseconds / 1000))
    elapsed_milliseconds=$((elapsed_milliseconds - 1000 * elapsed_seconds))
    elapsed_milliseconds=$(printf "%03g" $elapsed_milliseconds)
    first_line=$(cat "$filename".satoutput | head -n 1)
    echo "$mid $first_line in ${elapsed_seconds}.${elapsed_milliseconds} seconds"
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

script_end=$($date_cmd +%s%3N)
elapsed_milliseconds=$((script_end - script_start))
elapsed_seconds=$((elapsed_milliseconds / 1000))
elapsed_milliseconds=$((elapsed_milliseconds - 1000 * elapsed_seconds))
elapsed_milliseconds=$(printf "%03g" $elapsed_milliseconds)
echo "Found max clique in ${elapsed_seconds}.${elapsed_milliseconds} seconds"
