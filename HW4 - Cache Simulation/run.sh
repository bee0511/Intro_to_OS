#/bin/bash

problem=4-1
# problem=4-2_LFU
# problem=4-2_LRU

make "$problem"

if [[ "$problem" = "4-1" ]]; then
    for i in {1..2} ; do
        echo "Running testcase $i"
        ./4-1 < "./testcase/part1_case${i}.txt" > "./output/part1_case${i}.txt"
        if diff -q "./output/part1_case${i}.txt" "./testcase/part1_ans${i}.txt" > /dev/null; then
            echo "Testcase $i passed"
        else
            echo "Testcase $i failed"
            echo "Expected:"
            cat "./testcase/part1_ans${i}.txt"
            echo "Output:"
            cat "./output/part1_case${i}.txt"
        fi
    done
elif [[ "$problem" = "4-2_LFU" ]]; then
    for i in {1..2} ; do
        echo "Running testcase $i"
        ./4-2_LFU < "./testcase/part2_case${i}.txt" > "./output/part2_LFU_case${i}.txt"
        if diff -q "./output/part2_LFU_case${i}.txt" "./testcase/part2_LFU_ans${i}.txt" > /dev/null; then
            echo "Testcase $i passed"
        else
            echo "Testcase $i failed"
            echo "Expected:"
            cat "./testcase/part2_LFU_ans${i}.txt"
            echo "Output:"
            cat "./output/part2_LFU_case${i}.txt"
        fi
    done
elif [[ "$problem" = "4-2_LRU" ]]; then
    for i in {1..2} ; do
        echo "Running testcase $i"
        ./4-2_LRU < "./testcase/part2_case${i}.txt" > "./output/part2_LRU_case${i}.txt"
        if diff -q "./output/part2_LRU_case${i}.txt" "./testcase/part2_LRU_ans${i}.txt" > /dev/null; then
            echo "Testcase $i passed"
        else
            echo "Testcase $i failed"
            echo "Expected:"
            cat "./testcase/part2_LRU_ans${i}.txt"
            echo "Output:"
            cat "./output/part2_LRU_case${i}.txt"
        fi
    done
fi