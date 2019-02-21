#!/bin/bash
echo $#
count=0
sum=0.0
for ARG in $@
do 
    count=$(($count+1));
    sum=$(($sum+$ARG));
    echo $sum
done
result=$(1.0*$sum/$count)
echo "Average is $resulctat"