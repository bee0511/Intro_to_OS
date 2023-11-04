#!/bin/sh

case_num=3

while getopts "t:" op 2>/dev/null; do
  case $op in
    t)
      num_threads=$OPTARG
      ;;
    *)
      echo "Default"
      ;;
  esac
done

g++ hw3-4_serial.cpp -o hw3-3 -lpthread
test -e hw3-3 || exit 255
time -p -o "$num_threads"threads.txt ./hw3-3 -t $num_threads < testcase/case"$case_num".txt
time -p -o 1thread.txt ./hw3-3 -t 1 < testcase/case"$case_num".txt

multi=$(cat "$num_threads"threads.txt | grep "real" | cut -c 6-)
single=$(cat 1thread.txt | grep "real" | cut -c 6-)

rm "$num_threads"threads.txt 1thread.txt
# Check if $multi and $single are not empty
if [ -n "$multi" ] && [ -n "$single" ]; then
  echo "Total speed up is $(echo "$single/$multi" | bc -l)"
else
  echo "Error: Unable to calculate speed up. Check the 'real' output in time."
fi

rm hw3-3
