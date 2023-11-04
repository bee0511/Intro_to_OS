#!/bin/sh

case_num=3
filename=hw3-4_serial.cpp

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

g++ "$filename" -o tmp -lpthread
test -e tmp || exit 255
multi_ans=$(time -p -o "$num_threads"threads.txt ./tmp -t $num_threads < testcase/case"$case_num".txt)
single_ans=$(time -p -o 1thread.txt ./tmp -t 1 < testcase/case"$case_num".txt)
ans=$(cat answer/case"$case_num".txt)
if [ "$multi_ans" -ne "$ans" ]; then
  echo "Answer of multiple thread is wrong, the answer of multiple thread is $multi_ans"
fi
if [ "$single_ans" -ne "$ans" ]; then
  echo "Answer of single thread is wrong, the answer of single thread is $single_ans"
fi

multi=$(cat "$num_threads"threads.txt | grep "real" | cut -c 6-)
single=$(cat 1thread.txt | grep "real" | cut -c 6-)
echo "Mutithread running time: $multi"
echo "Singlethread running time: $single"
rm "$num_threads"threads.txt 1thread.txt

if [ "$single" != "0.00" ]; then
  echo "Total speed up is $(echo "$single/$multi" | bc -l)"
else
  echo "Error: Unable to calculate speed up."
fi

rm tmp
