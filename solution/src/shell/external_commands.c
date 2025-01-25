#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>  
#include <signal.h> 
#include "../../include/shell/external_commands.h"

int child_function(void* arg) {
    struct clone_args* clone_arg = (struct clone_args*)arg;
    char* command = clone_arg->command;
    char* args = clone_arg->args;

    char* args_array[64];
    int i = 0;
    args_array[i++] = command;
    char* token = strtok(args, " ");
    while (token != NULL) {
        args_array[i++] = token;
        token = strtok(NULL, " ");
    }
    args_array[i] = NULL;

    execvp(command, args_array);
    perror("execvp");
    return EXIT_FAILURE;
}

bool handle_external_command(char* command, char* args) {
    // Проверяем, если команда является встроенной, то не вызываем execvp
    if (strcmp(command, "echo") == 0 || strcmp(command, "cd") == 0 || strcmp(command, "exit") == 0 || strcmp(command, "pwd") == 0) {
        return false;  // Уже обработано в handle_command
    }

    // Если команда не встроенная, выполняем её через execvp
    void* child_stack = malloc(STACK_SIZE);
    if (child_stack == NULL) {
        perror("malloc");
        return false;
    }

    struct clone_args args_data = { command, args };
    pid_t pid = clone(child_function, (char*) child_stack + STACK_SIZE, SIGCHLD, &args_data);
    if (pid == -1) {
        perror("clone");
        free(child_stack);
        return false;
    }

    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        free(child_stack);
        return false;
    }

    free(child_stack);
    return true;
}

