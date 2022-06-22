#include <stddef.h>
#include <stdint.h>

void salsa20_core(uint32_t output[16], const uint32_t input[16]);

void salsa20_crypt(size_t mlen, const uint8_t msg[], uint8_t cipher[], uint32_t key[8], uint64_t iv);