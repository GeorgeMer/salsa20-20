#ifndef SALSACOREV1_H
#define SALSACOREV1_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void salsa_core_v1(uint32_t output[16], const uint32_t input[16]);
uint32_t littleendian(uint8_t input[4]);

#endif