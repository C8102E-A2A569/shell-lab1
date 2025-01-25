#define _GNU_SOURCE
#include <stdlib.h>
#include "../include/dedup/dedup.h"

static int compare_ints(const int *a, const int *b);

size_t deduplicate_array(int *array, size_t size) {
    if (!array || size == 0) return 0;

    qsort(array, size, sizeof(int), (int (*)(const void *, const void *))&compare_ints);

    size_t unique_count = 1;
    for (size_t i = 1; i < size; i++) {
        if (array[i] != array[i - 1]) {
            array[unique_count++] = array[i];
        }
    }
    return unique_count;
}

static int compare_ints(const int *a, const int *b) {
    return (*a > *b) - (*a < *b);
}


