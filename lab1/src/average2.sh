#!/bin/bash
echo $#
count=0
sum=0
for ARG in $@
do
	count=$((count+1));
sum=$((sum+ARG));
done
result=$($sum/$count)
echo "Average is $result"
