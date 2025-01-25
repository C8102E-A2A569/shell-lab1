#!/bin/bash

echo "Testing deduplication functionality (dedup)"

# Параметры теста
iterations=1
array_size=8
generated_file="../solution/build/test_array.bin"

# Генерация массива в бинарный файл
../solution/build/lab1 generate-array $array_size $generated_file

# Выполняем программу с аргументами для dedup
actual_output=$("../build/solution/lab1" dedup $iterations $array_size $generated_file | grep -v "Execution time")

# Выводим результат для отладки
echo "Actual output:"
echo "$actual_output"

unique_count=$(echo "$actual_output" | grep -oP 'Unique elements = \K[0-9]+')
if [[ "$unique_count" -gt 0 && "$unique_count" -le "$array_size" ]]; then
    echo "Test passed: dedup works correctly."
    rm -f $generated_file
    exit 0
else
    echo "Test failed: dedup produced an invalid count of unique elements."
    rm -f $generated_file
    exit 1
fi
