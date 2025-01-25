#ifndef TOOLS_H
#define TOOLS_H
#define CD cd
#define BUFFER_SIZE 1024
enum OPERATION_STATUS{
    SUCCESS,
    ERROR,
    NOT_FOUND
};

enum OPERATION_STATUS get_env_var(const char *var_name, char **ret);

void prepare_command(char* buffer, char** command, char** args);

void string_from_string_arr(char* out_string, size_t out_size, int arr_len, int start_index, char** arr);

double calculate_time_diff(struct timespec start, struct timespec end);

#endif
