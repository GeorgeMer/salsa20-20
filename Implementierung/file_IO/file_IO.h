#define _POSIX_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

extern size_t mlen;
uint8_t *read_file(const char *path);
void write_file(const char *path, uint8_t *cipher);