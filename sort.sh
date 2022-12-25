#!/bin/bash

array=("$@")

function sort(){
declare -n sortarr="$1"
temp=0
len=$((${#sortarr[@]}-1))

for (( i=len; i>0; i-- ))
do
        for (( j=0; j<i; j++ ))
        do
                if [ ${sortarr[$j]} -gt ${sortarr[$j+1]} ]
		then
                        temp=${sortarr[$j]}
                        sortarr[$j]=${sortarr[$j+1]}
                        sortarr[$j+1]=$temp
                fi
        done
done
}

sort array
echo "${array[*]}"
