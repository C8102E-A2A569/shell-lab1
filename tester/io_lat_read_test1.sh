#!/bin/bash

echo "Assert io-lat-read works correctly"

# Путь к директории, где нужно создать файл
target_directory="../build/solution"
test_file="testfile"

# Создаем временный файл для теста
dd if=/dev/urandom of=$test_file bs=1M count=10 > /dev/null 2>&1

# Параметры теста
block_size=4096
iterations=100

# Выполняем программу с правильным порядком аргументов
actual_output=$("../build/solution/lab1" io-lat-read $iterations $block_size $test_file | grep "Average latency")

if [[ "$actual_output" =~ Average\ latency:\ [0-9]+\.[0-9]+ ]]; then
    echo "Test passed: io-lat-read works correctly."
    rm -f $test_file  # Удаляем файл только при успешном завершении
    exit 0
else
    echo "Test failed: io-lat-read does not work correctly."
    echo "Actual: $actual_output"
    # Не удаляем файл, оставляем для анализа
    exit 1
fi
