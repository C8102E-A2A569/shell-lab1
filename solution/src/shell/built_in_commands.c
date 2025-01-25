#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/shell/tools.h"
#include "dedup/dedup.h"
#include "io-lat-read/tools.h"
#include "io-lat-read/io_lat_read.h"

static void handle_echo(const char* args) {
    printf("%s\n", args);
}

static void handle_cd(const char* path) {
    if (chdir(path) == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            setenv("PWD", cwd, 1); 
        } else {
            printf("getcwd failed");
        }
    }else{
        printf("Error: No such file or directory.\n");
    }
}

static void handle_exit() {
    exit(0);
}

static void handle_pwd() {
    char* cur_dir;
    get_env_var("PWD", &cur_dir);
    printf("%s\n", cur_dir);
}

bool handle_command(const char *command, const char *args) {
    // Обработка встроенных команд
    if (strcmp(command, "echo") == 0) {
        handle_echo(args);
    } else if (strcmp(command, "cd") == 0) {
        handle_cd(args);
    } else if (strcmp(command, "exit") == 0) {
        handle_exit();
    } else if (strcmp(command, "pwd") == 0) {
        handle_pwd();
    } else if (strcmp(command, "io-lat-read") == 0) {
        // Обработка команды io-lat-read
        char *filename = strtok(NULL, " ");
        size_t block_size = parse_size(strtok(NULL, " "));
        size_t iterations = parse_size(strtok(NULL, " "));
        measure_io_latency(filename, block_size, iterations);
    } else if (strcmp(command, "dedup") == 0) {
        // Обработка команды dedup
        size_t array_size = parse_size(strtok(NULL, " "));
        int *array = malloc(array_size * sizeof(int));
        if (!array) {
            perror("malloc");
            return true;
        }
        for (size_t i = 0; i < array_size; i++) {
            array[i] = rand() % 1000;
        }
        size_t unique_count = deduplicate_array(array, array_size);
        printf("Unique elements: %zu\n", unique_count);
        free(array);
    } else {
        // Команда не является встроенной, возвращаем false для обработки внешней команды
        return false;
    }
    fflush(stdout);
    return true;
}


