#ifndef SALSACRYPT_H
#define SALSACRYPT_H

#include <stddef.h>

void salsa20_crypt(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv);

#endif