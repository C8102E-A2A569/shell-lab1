#!/bin/bash

echo "Assert built-in command 'ls' works correctly"

expected_output=$(ls)
actual_output=$("../build/solution/lab1" shell ls | grep -v "Execution time")

if [ "$expected_output" = "$actual_output" ]; then
    echo "Test passed: 'ls' works correctly."
else
    echo "Test failed: 'ls' does not work correctly."
    echo "Expected: $expected_output"
    echo "Actual: $actual_output"
fi
