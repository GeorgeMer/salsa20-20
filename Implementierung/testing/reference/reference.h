#ifndef REFERENCE_H
#define REFERENCE_H

#include <stddef.h>
#include <stdint.h>

void salsa_crypt_reference(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv);

#endif