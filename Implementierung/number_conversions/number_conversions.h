#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

uint64_t convert_string_to_uint64_t(const char *string);
uint32_t convert_string_to_uint32_t(const char *string, int base);
void convert_string_to_uint32_t_array(const char *string, uint32_t key[8]);