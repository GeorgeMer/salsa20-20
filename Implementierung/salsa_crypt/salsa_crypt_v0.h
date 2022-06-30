#ifndef SALSACRYPTV0_H
#define SALSACRYPTV0_H

#include <stddef.h>
#include <stdint.h>

void salsa_crypt_v0(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv);

#endif