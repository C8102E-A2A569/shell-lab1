#!/bin/bash

echo "Assert built-in command 'pwd' works correctly"

expected_output=$(pwd)
actual_output=$("../build/solution/lab1" shell pwd | grep -v "Execution time")

if [ "$expected_output" = "$actual_output" ]; then
    echo "Test passed: 'pwd' works correctly."
    exit 0
else
    echo "Test failed: 'pwd' does not work correctly."
    echo "Expected: $expected_output"
    echo "Actual: $actual_output"
    exit 1
fi
