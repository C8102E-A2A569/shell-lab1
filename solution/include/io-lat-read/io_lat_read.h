#ifndef IO_LAT_READ_H
#define IO_LAT_READ_H
#include <stddef.h>
void measure_io_latency(const char *file_path, size_t block_size, size_t iterations);
#endif
