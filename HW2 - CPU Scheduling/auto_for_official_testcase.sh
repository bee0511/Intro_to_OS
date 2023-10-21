#!/bin/sh

# Compile your program
if g++ -o hw2 ./hw2_110550164.cpp; then
  # Run your program and redirect its output to a file
  ./hw2 < ./testcase/part_1/t1.txt > program_output.txt

  # Compare the program output with the expected answer
  if diff program_output.txt ./answer/part_1/a1.txt; then
    echo "Test passed."
  else
    echo "Test failed. Output does not match the expected answer."
  fi

  # Clean up the compiled program and temporary output file
  rm hw2 program_output.txt
else
  echo "Compilation failed. Please check your code."
fi
