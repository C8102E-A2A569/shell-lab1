#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/io-lat-read/array_generator.h"

int generate_array(size_t size, const char* filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    srand((unsigned int)(time(NULL) ^ clock()));
    int *array = malloc(size * sizeof(int));
    if (!array) {
        perror("malloc");
        fclose(file);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < size; i++) {
        if (rand() % 10 == 0 && i > 0) { 
            array[i] = array[rand() % i];
        } else {
            array[i] = rand() % 1000000;
        }
        fwrite(&array[i], sizeof(int), 1, file);
    }

    free(array);
    fclose(file);
    printf("Array generated and saved to file: %s\n", filename);
    return 0;
}