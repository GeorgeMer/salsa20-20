#ifndef PRINTTESTS_H
#define PRINTTESTS_H

#include <stddef.h>
#include <stdint.h>

void print_char_arr(size_t len, const uint8_t arr[len]);

void print_arr_8bit(size_t len, const uint8_t arr[len]);

void print_arr_32bit(size_t len, const uint32_t arr[len]);

void print_crypt(const uint32_t arr[16]);
#endif
