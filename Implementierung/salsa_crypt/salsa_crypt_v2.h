#ifndef SALSACRYPTV2_H
#define SALSACRYPTV2_H

#include <stddef.h>
#include <stdint.h>

void salsa20_crypt_v2(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv);

#endif