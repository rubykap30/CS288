#!/bin/bash
function reverse() {
        declare -n arr="$1" rev="$2"
        for i in "${arr[@]}"
        do
                rev=("$i" "${rev[@]}")
        done
}
dir=$1
declare -a ex=( "${dir}"/* )
declare -a rev1
reverse ex rev1
for i in "${rev1[@]}"
do
        echo $i
done
