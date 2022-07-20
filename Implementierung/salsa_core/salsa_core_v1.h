#ifndef SALSACOREV1_H
#define SALSACOREV1_H

#include <stdint.h>

void salsa20_core_v1(uint32_t output[16], const uint32_t input[16]);

#endif