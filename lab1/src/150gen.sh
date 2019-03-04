#!/bin/bash
a=0
for ((i=1; i <= 150; i++))
do
    tr -cd 1-9 </dev/urandom | head -c1 >> numbers.txt
    echo ''>>numbers.txt
done