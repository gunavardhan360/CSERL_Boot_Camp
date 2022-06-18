#!/bin/sh

Animals=("man" "bear" "pig" "dog" "cat" "sheep")

for (( counter=0; counter<6; counter++ ))
do
echo -n "${Animals[counter]}"
printf "\n"
done
