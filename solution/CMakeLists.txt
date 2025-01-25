cmake_minimum_required(VERSION 3.10)
project(Lab1)

# Указываем исходные файлы
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        src/*.c
        src/*.h
        include/*.h
        src/io-lat-read/*.c
        include/io-lat-read/*.h
        src/shell/*.c
        include/shell/*.h
        src/dedup/*.c
        include/dedup/*.h
)

# Создаем исполняемый файл
add_executable(lab1 ${sources})

# Указываем директории для заголовочных файлов
target_include_directories(lab1 PRIVATE src include)
