#ifndef ASSERTS_H
#define ASSERTS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

bool assertEqualsArrays_8bit(uint8_t expected[], uint64_t xlen, uint8_t actual[], uint64_t alen);
bool assertEqualsArrays_16bit(uint16_t expected[], uint64_t xlen, uint16_t actual[], uint64_t alen);
bool assertEqualsArrays_32bit(uint32_t expected[], uint64_t xlen, uint32_t actual[], uint64_t alen);
bool assertEqualsArrays_64bit(uint64_t expected[], uint64_t xlen, uint64_t actual[], uint64_t alen);

int randomInt(int from, int to);
#endif