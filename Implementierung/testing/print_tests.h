#ifndef PRINTTESTS_H
#define PRINTTESTS_H

#include <stddef.h>
#include <stdint.h>

void print_char_arr(const uint8_t arr[], size_t len);


void print_arr_8bit(const uint8_t arr[], size_t len);


void print_arr_32bit(const uint32_t arr[], size_t len);


void print_key(const uint32_t arr[]);

void print_crypt(const uint32_t arr[]);


#endif
