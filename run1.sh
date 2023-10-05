#!/bin/bash

if [ $# != 1 ]; then
    echo "Filename required as argument"
    exit 1
fi

filename="$1"

./main.out "$filename"
