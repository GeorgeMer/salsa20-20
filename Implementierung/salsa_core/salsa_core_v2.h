#ifndef SALSACOREV2_H
#define SALSACOREV2_H

#include <stdint.h>

void salsa20_core_v2(uint32_t output[16], const uint32_t input[16]);

#endif