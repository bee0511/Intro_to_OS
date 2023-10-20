#!/bin/sh

# Compile your program
g++ -o hw2 ./hw2_110550164.cpp
i=3
while [ $i -le 3 ]; do
    ./hw2 < ./mytestcase/testcase/tc_$i.txt > program_output.txt
    # Compare the program output with the expected answer
    if diff program_output.txt ./mytestcase/ans/ans_$i.txt; then
      echo "Test $i passed."
    else
      echo "Test $i failed. Output does not match the expected answer."
    fi
    # rm program_output.txt
    i=$((i + 1))
done

rm hw2
