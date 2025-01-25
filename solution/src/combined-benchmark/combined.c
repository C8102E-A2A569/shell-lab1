#define _GNU_SOURCE
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../../include/io-lat-read/array_generator.h"
#include "../../include/io-lat-read/io_lat_read.h"
#include "../../include/dedup/dedup.h"
#include "../../include/combined-benchmark/combined.h"
#include "../../include/shell/tools.h"

void *start_dedup_benchmark_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    size_t array_size = data->array_size;
    int *array = malloc(array_size * sizeof(int));
    if (!array) {
        perror("malloc");
        return NULL;
    }

    // Генерация массива с дубликатами
    const char *generated_file = data->filename;
    if (generate_array(array_size, generated_file) != 0) {
        perror("Failed to generate array");
        free(array);
        return NULL;
    }

    // Чтение массива из файла
    FILE *file = fopen(generated_file, "rb");
    if (!file) {
        perror("fopen");
        free(array);
        return NULL;
    }
    fread(array, sizeof(int), array_size, file);
    fclose(file);

    size_t unique_count = deduplicate_array(array, array_size);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = calculate_time_diff(start_time, end_time);

    printf("Dedup Execution time: %.6f seconds\n", elapsed_time);
    printf("Unique elements: %zu\n", unique_count);
    fflush(stdout);

    free(array);
    return NULL;
}

void *start_io_lat_read_benchmark_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    measure_io_latency(data->filename, data->array_size, data->iterations);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = calculate_time_diff(start_time, end_time);

    printf("IO Latency Execution time: %.6f seconds\n", elapsed_time);
    fflush(stdout);

    return NULL;
}

void start_combined_benchmark() {
    int dedup_array_size = 1000000;
    int dedup_iterations = 1;
    char *dedup_filename = "../../build/solution/generated_array.bin";
  
    size_t io_lat_read_iterations = 1000;
    size_t io_lat_read_block_size = 4096;
    char *io_lat_read_filename = "../../build/solution/generated_array.bin";

    pthread_t dedup_thread, io_lat_read_thread;

    ThreadData dedup_data = {
        .type = 0,  // Dedup benchmark
        .array_size = dedup_array_size,
        .iterations = dedup_iterations,
        .target = 0,  // Не используется в dedup
        .filename = dedup_filename
    };

    ThreadData io_lat_read_data = {
        .type = 1,  // IO Latency Read benchmark
        .array_size = io_lat_read_block_size,
        .iterations = io_lat_read_iterations,
        .target = 0,  // Не используется в io-lat-read
        .filename = io_lat_read_filename
    };

    pthread_create(&dedup_thread, NULL, start_dedup_benchmark_thread, &dedup_data);
    pthread_create(&io_lat_read_thread, NULL, start_io_lat_read_benchmark_thread, &io_lat_read_data);

    pthread_join(dedup_thread, NULL);
    pthread_join(io_lat_read_thread, NULL);
}
