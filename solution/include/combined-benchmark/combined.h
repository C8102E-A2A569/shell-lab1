#ifndef COMBINED_H
#define COMBINED_H
#include <pthread.h>

typedef struct {
    int type;  // Тип бенчмарка: 0 - dedup, 1 - io_lat_read
    int array_size;
    int iterations;
    int target;  // Для dedup не используется
    char *filename;  // Для io_lat_read и dedup
} ThreadData;

void start_combined_benchmark();

#endif
