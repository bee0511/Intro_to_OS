#/bin/bash

# problem=4-1
problem=4-2_LFU
# problem=4-2_LRU

make "$problem"

if [[ "$problem" = "4-1" ]]; then
    for i in {1..2} ; do
        echo "Running testcases $i"
        ./4-1 < "./testcases/part1_case${i}.txt" > "./output/part1_case${i}.txt"
        if diff -q "./output/part1_case${i}.txt" "./testcases/part1_ans${i}.txt" > /dev/null; then
            echo "Testcase $i passed"
        else
            echo "Testcase $i failed"
            echo "Expected:"
            cat "./testcases/part1_ans${i}.txt"
            echo -e "\nOutput:"
            cat "./output/part1_case${i}.txt"
        fi
    done
elif [[ "$problem" = "4-2_LFU" ]]; then
    for i in {1..2} ; do
        echo -e "\nRunning testcases $i"
        ./4-2_LFU < "./testcases/part2_case${i}.txt" > "./output/part2_LFU_case${i}.txt"
        if diff -q "./output/part2_LFU_case${i}.txt" "./testcases/part2_LFU_ans${i}.txt" > /dev/null; then
            echo "Testcase $i passed"
        else
            echo "Testcase $i failed"
            echo "Expected:"
            cat "./testcases/part2_LFU_ans${i}.txt"
            echo -e "\nOutput:"
            cat "./output/part2_LFU_case${i}.txt"
        fi
    done
elif [[ "$problem" = "4-2_LRU" ]]; then
    for i in {1..2} ; do
        echo -e "\nRunning testcases $i"
        ./4-2_LRU < "./testcases/part2_case${i}.txt" > "./output/part2_LRU_case${i}.txt"
        if diff -q "./output/part2_LRU_case${i}.txt" "./testcases/part2_LRU_ans${i}.txt" > /dev/null; then
            echo "Testcase $i passed"
        else
            echo "Testcase $i failed"
            echo "Expected:"
            cat "./testcases/part2_LRU_ans${i}.txt"
            echo -e "\nOutput:"
            cat "./output/part2_LRU_case${i}.txt"
        fi
    done
fi

echo -e "\nCleaning up"
make clean