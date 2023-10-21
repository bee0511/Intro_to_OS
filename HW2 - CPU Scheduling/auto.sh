#!/bin/sh

# Compile your program
g++ -o hw2 ./hw2_110550164.cpp
i=1
while [ $i -le 1000 ]; do
    ./hw2 < ./mytestcase/testcase/tc_$i.txt > ./myanswer/ans$i.txt
    # Compare the program output with the expected answer
    if diff ./myanswer/ans$i.txt ./mytestcase/ans/ans_$i.txt; then
      echo "Test $i passed."
      rm ./myanswer/ans$i.txt
    else
      echo "Test $i failed. Output does not match the expected answer."

    fi
    # rm program_output.txt
    i=$((i + 1))
done

rm hw2
