#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include "../../include/io-lat-read/io_lat_read.h"
#include "../../include/shell/tools.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>     
#include <fcntl.h>

void measure_io_latency(const char *file_path, size_t block_size, size_t iterations) {
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    char *buffer = malloc(block_size);
    if (!buffer) {
        perror("malloc");
        close(fd);
        return;
    }

    struct timespec start, end;
    double total_time = 0;

    for (size_t i = 0; i < iterations; i++) {
        off_t offset = (rand() % 1000) * block_size;
        lseek(fd, offset, SEEK_SET);

        clock_gettime(CLOCK_MONOTONIC, &start);
        read(fd, buffer, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);

        total_time += calculate_time_diff(start, end);
    }

    printf("Average latency: %.6f ms\n", (total_time / iterations) * 1000);

    free(buffer);
    close(fd);
}
