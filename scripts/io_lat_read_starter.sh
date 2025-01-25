#!/bin/bash

# Проверка наличия параметра (количество нагрузчиков)
if [ -z "$1" ]; then
  echo "Usage: $0 <number_of_load_testers>"
  exit 1
fi

# Количество нагрузчиков
NUM_LOADERS=$1

# Инициализация глобальных переменных
REAL_TIME=0
SYSTEM_TIME=0
USER_TIME=0
INVOLUNTARY_CS=0
VOLUNTARY_CS=0

# Функция для запуска нагрузчика
run_load_tester() {
  local index=$1
  local output=$(/usr/bin/time -v ../build/solution/lab1 io-lat-read 100000 512 2>&1)

  # Извлечение данных из вывода
  local system_time=$(echo "$output" | grep "System time" | awk '{print $4}')
  local user_time=$(echo "$output" | grep "User time" | awk '{print $4}')
  local involuntary_cs=$(echo "$output" | grep "Involuntary context switches" | awk '{print $4}')
  local voluntary_cs=$(echo "$output" | grep "Voluntary context switches" | awk '{print $4}')
  local real_time=$(echo "$user_time + $system_time" | bc)

  # Отладочная информация
  echo "Extracted values for load tester $index:"
  echo "Real time: $real_time, System time: $system_time, User time: $user_time"
  echo "Involuntary context switches: $involuntary_cs, Voluntary context switches: $voluntary_cs"

  # Проверка на пустые значения и продолжение, если есть пустые
  if [ -z "$real_time" ] || [ -z "$system_time" ] || [ -z "$user_time" ] || [ -z "$involuntary_cs" ] || [ -z "$voluntary_cs" ]; then
    echo "Warning: One or more extracted values are empty for load tester $index. Skipping this tester."
    return 0  # Просто пропустим этот тестер
  fi

  # Запись данных в файл
  echo "$real_time $system_time $user_time $involuntary_cs $voluntary_cs" > "load_tester_$index.txt"
  echo "Load tester $index completed."
}

# Запуск нагрузчиков параллельно
for ((i=1; i<=$NUM_LOADERS; i++)); do
  run_load_tester $i &
done

# Ожидание завершения всех нагрузчиков
wait

# Чтение результатов и подсчет средних значений
REAL_TIME_SUM=0
SYSTEM_TIME_SUM=0
USER_TIME_SUM=0
INVOLUNTARY_CS_SUM=0
VOLUNTARY_CS_SUM=0

for ((i=1; i<=$NUM_LOADERS; i++)); do
  # Чтение значений из файлов
  read real_time system_time user_time involuntary_cs voluntary_cs < "load_tester_$i.txt"

  REAL_TIME_SUM=$(echo "$REAL_TIME_SUM + $real_time" | bc)
  SYSTEM_TIME_SUM=$(echo "$SYSTEM_TIME_SUM + $system_time" | bc)
  USER_TIME_SUM=$(echo "$USER_TIME_SUM + $user_time" | bc)
  INVOLUNTARY_CS_SUM=$(echo "$INVOLUNTARY_CS_SUM + $involuntary_cs" | bc)
  VOLUNTARY_CS_SUM=$(echo "$VOLUNTARY_CS_SUM + $voluntary_cs" | bc)
done

# Подсчет средних значений
avg_real_time=$(echo "scale=2; $REAL_TIME_SUM / $NUM_LOADERS" | bc)
avg_system_time=$(echo "scale=2; $SYSTEM_TIME_SUM / $NUM_LOADERS" | bc)
avg_user_time=$(echo "scale=2; $USER_TIME_SUM / $NUM_LOADERS" | bc)
avg_involuntary_cs=$(echo "scale=2; $INVOLUNTARY_CS_SUM / $NUM_LOADERS" | bc)
avg_voluntary_cs=$(echo "scale=2; $VOLUNTARY_CS_SUM / $NUM_LOADERS" | bc)

# Вывод таблицы со средними значениями
echo "---------------------------------------------"
echo "| Metric                          | Average  |"
echo "---------------------------------------------"
echo "| Real time                       | $avg_real_time |"
echo "| System time                     | $avg_system_time |"
echo "| User time                       | $avg_user_time |"
echo "| Involuntary context switches    | $avg_involuntary_cs |"
echo "| Voluntary context switches      | $avg_voluntary_cs |"
echo "---------------------------------------------"

rm -rf load_tester_*
