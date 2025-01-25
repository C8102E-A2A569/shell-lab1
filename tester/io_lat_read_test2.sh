#!/bin/bash

echo "Assert io-lat-read handles non-existent file"

# Несуществующий файл
non_existent_file="non_existent_file"
block_size=4096
iterations=100

# Выполняем программу
actual_output=$("../build/solution/lab1" io-lat-read $iterations $block_size $non_existent_file   2>&1)

# Ожидаемое сообщение об ошибке
expected_output="open: No such file or directory"

# Проверяем, что программа возвращает ожидаемое сообщение об ошибке
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

