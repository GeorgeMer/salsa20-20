#ifndef SALSACRYPTV1_H
#define SALSACRYPTV1_H

#include <stddef.h>

void salsa20_crypt_v1(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv);

#endif