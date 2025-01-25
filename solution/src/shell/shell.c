#define _POSIX_C_SOURCE 199309L
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "../../include/shell/external_commands.h"
#include "../../include/shell/built_in_commands.h"
#include "../../include/shell/tools.h"
void read_command(char* buffer){
    ssize_t bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
        perror("read");
        return;
    }

    buffer[bytesRead] = '\0';
}

void single_command_execution(char* buffer){
    
    char* command = NULL;
    char* args = NULL;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    prepare_command(buffer, &command, &args);

    bool command_is_built_in = handle_command(command, args);

    if (!command_is_built_in) {
        handle_external_command(command, args);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = calculate_time_diff(start_time, end_time);
    
    printf("Execution time: %.6f seconds\n", elapsed_time);
    fflush(stdout);
}

void start_shell(char* buffer){
    while (true) {
        char* cur_dir;
        get_env_var("PWD", &cur_dir);
        printf("%s> ", cur_dir);
        fflush(stdout);
        read_command(buffer);
        single_command_execution(buffer);
    }
}
