#ifndef EXTERNAL_COMMANDS_H
#define EXTERNAL_COMMANDS_H

bool handle_external_command(char* command, char* args);

#define STACK_SIZE (1024 * 1024)

struct clone_args {
    char* command;
    char* args;
};
#endif
