#!/bin/bash

for i in {A..Z}
do
	for j in {A..Z}
	do
		wget https://en.wikipedia.org/wiki$i$j -O $i$j.html
		lynx -dump -nolist $i$j.html >$i$j.txt
		tr -s '[:space:]' '\n' < $i$j.txt | tr 'A-Z' 'a-z' | grep [[:alpha:]] >> newfile.txt
	done
done

sort < newfile.txt| uniq -c|sort -nr| head -n 5
