#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

extern size_t mlen;
const uint8_t *read_file(const char *path);
static void write_file(const char *path, uint8_t *cipher);