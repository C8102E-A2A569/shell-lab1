#ifndef SHELL_H
#define SHELL_H
#define BUFFER_SIZE 1024

void read_command(char* buffer);

void single_command_execution(char* buffer);

void start_shell(char* buffer);

#endif
