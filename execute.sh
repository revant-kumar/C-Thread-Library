#!/bin/bash

# Number of times to execute the command
n=10

# Command to execute
command="./main"

# Loop to execute the command n times
for ((i=1; i<=n; i++))
do
   echo "Execution $i"
   $command
done
