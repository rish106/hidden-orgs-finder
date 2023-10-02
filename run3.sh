#!/usr/bin/env sh

if [ $# != 1 ]; then
    echo "Filename required as argument"
    exit 1
fi

filename="$1"
n=$(cat "$filename".graph | head -n 1 | awk '{print $1}')
left=0
right=$n

while [[ $left < $right ]]; do
    middle=$(($(($left + $right)) / 2))
    ./main.out "$filename" $middle
    minisat "$filename".satinput "$filename".satoutput > /dev/null
    first_line=$(cat "$filename".satoutput | head -n 1)
    echo $middle
    echo $first_line
    if [[ $(echo "$first_line" | grep "UNSAT") ]]; then
        right=$middle
    else
        left=$(($middle + 1))
    fi
done
