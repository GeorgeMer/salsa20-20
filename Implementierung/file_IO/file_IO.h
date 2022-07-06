#define _POSIX_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

extern size_t mlen;
const uint8_t *read_file(const char *path);
void write_file(const char *path, uint8_t *cipher);