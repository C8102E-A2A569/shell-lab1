#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/shell/shell.h"
#include "../include/shell/tools.h"
#include "../include/io-lat-read/io_lat_read.h"
#include "../include/io-lat-read/array_generator.h"
#include "../include/dedup/dedup.h"
#include "../include/io-lat-read/tools.h"
#include "../include/combined-benchmark/combined.h"

int main(int argc, char **argv) {
    char buffer[BUFFER_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  To use shell: lab1 shell || lab1 shell <command>\n");
        fprintf(stderr, "  To use io-lat-read benchmark: lab1 io-lat-read <iterations> <block size> <filename>\n");
        fprintf(stderr, "  To use dedup benchmark: lab1 dedup <iterations> <array size> [filename]\n");
        fprintf(stderr, "  To generate an array file: lab1 generate-array <array size> <filename>\n");
        fprintf(stderr, "  To use combined benchmark: lab1 combined-benchmark\n");
        exit(EXIT_FAILURE);
    }

    bool shell_mode = strcmp(argv[1], "shell") == 0;
    bool io_lat_read_mode = strcmp(argv[1], "io-lat-read") == 0;
    bool dedup_mode = strcmp(argv[1], "dedup") == 0;
    bool generate_array_mode = strcmp(argv[1], "generate-array") == 0;
    bool combined_benchmark_mode = strcmp(argv[1], "combined-benchmark") == 0;


    if (shell_mode && argc == 2) {
        start_shell(buffer);
    } else if (shell_mode) {
        string_from_string_arr(buffer, BUFFER_SIZE, argc, 2, argv);
        single_command_execution(buffer);
    }


    if (io_lat_read_mode && argc == 5) {
        size_t iterations = parse_size(argv[2]);
        size_t block_size = parse_size(argv[3]);
        const char *filename = argv[4];

        measure_io_latency(filename, block_size, iterations);
    } else if (io_lat_read_mode) {
        fprintf(stderr, "Invalid arguments for io-lat-read benchmark:\n");
        fprintf(stderr, "Usage: lab1 io-lat-read <iterations> <block size> <filename>\n");
        exit(EXIT_FAILURE);
    }


    if (dedup_mode && argc >= 4) {
        size_t iterations = parse_size(argv[2]);
        size_t array_size = parse_size(argv[3]);
        size_t unique_count = 0;

        if (argc == 5) {
            const char *filename = argv[4];

            for (size_t i = 0; i < iterations; i++) {
                int *array = malloc(array_size * sizeof(int));
                if (!array) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }

                FILE *file = fopen(filename, "rb");
                if (!file) {
                    perror("fopen");
                    free(array);
                    exit(EXIT_FAILURE);
                }

                size_t elements_read = fread(array, sizeof(int), array_size, file);
                if (elements_read != array_size) {
                    fprintf(stderr, "Error: Expected to read %zu elements, but only %zu were read.\n", array_size, elements_read);
                    free(array);
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                fclose(file);

                unique_count = deduplicate_array(array, array_size);
                free(array);
            }
        } else {
            for (size_t i = 0; i < iterations; i++) {
                int *array = malloc(array_size * sizeof(int));
                if (!array) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }

                const char *generated_file = "../solution/build/generated_array.bin";
                if (generate_array(array_size, generated_file) != 0) {
                    fprintf(stderr, "Failed to generate array\n");
                    free(array);
                    exit(EXIT_FAILURE);
                }

                // Read the generated array from file
                FILE *file = fopen(generated_file, "rb");
                if (!file) {
                    perror("fopen");
                    free(array);
                    exit(EXIT_FAILURE);
                }
                fread(array, sizeof(int), array_size, file);
                fclose(file);

                unique_count = deduplicate_array(array, array_size);
                free(array);
            }
        }

        printf("Total iterations: %zu, Unique elements = %zu\n", iterations, unique_count);
    }


    if (generate_array_mode && argc == 4) {
        size_t array_size = parse_size(argv[2]);
        const char *filename = argv[3];

        if (generate_array(array_size, filename) != 0) {
            fprintf(stderr, "Failed to generate array file\n");
            exit(EXIT_FAILURE);
        }
    } else if (generate_array_mode) {
        fprintf(stderr, "Invalid arguments for generate-array:\n");
        fprintf(stderr, "Usage: lab1 generate-array <array size> <filename>\n");
        exit(EXIT_FAILURE);
    }


    if (combined_benchmark_mode && argc == 2) {
        start_combined_benchmark();
    } else if (combined_benchmark_mode) {
        fprintf(stderr, "Invalid arguments for combined benchmark:\n");
        fprintf(stderr, "Usage: lab1 combined-benchmark\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
