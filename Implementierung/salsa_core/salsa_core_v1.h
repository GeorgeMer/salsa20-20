#ifndef SALSACOREV1_H
#define SALSACOREV1_H

#include <stdint.h>

void salsa_core_v1(uint32_t output[16], const uint32_t input[16]);
uint32_t littleendian(uint8_t input[4]);
void littleendian_inv(uint8_t *out, uint32_t n);

#endif