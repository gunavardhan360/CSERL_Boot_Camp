#!/bin/bash
## seq command 

mkdir ./root

for (( counter=1; counter<6; counter++ ))
do
	mkdir ./root/dir$counter
	for (( i=1; i<5; i++ ))
	do
		touch ./root/dir$counter/file$i.txt
		exec 3<> ./root/dir$counter/file$i.txt
			for (( j=1; j<i+1; j++))
			do
				echo "$i" >&3
			done
		exec 3>&-
	done
done

