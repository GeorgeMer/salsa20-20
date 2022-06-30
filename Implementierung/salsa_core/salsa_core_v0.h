#ifndef SALSACOREV0_H
#define SALSACOREV0_H

#include <stddef.h>
#include <stdint.h>

void salsa20_core(uint32_t output[16], const uint32_t input[16]);

#endif