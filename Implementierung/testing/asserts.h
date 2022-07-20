#ifndef ASSERTS_H
#define ASSERTS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

bool assertEqualsArrays_8bit(uint64_t xlen, const uint8_t expected[xlen], uint64_t alen, uint8_t actual[alen]);
bool assertEqualsArrays_16bit(uint64_t xlen, const uint16_t expected[xlen], uint64_t alen, uint16_t actual[alen]);
bool assertEqualsArrays_32bit(uint64_t xlen, const uint32_t expected[xlen], uint64_t alen, uint32_t actual[alen]);
bool assertEqualsArrays_64bit(uint64_t xlen, const uint64_t expected[xlen], uint64_t alen, uint64_t actual[alen]);

#endif