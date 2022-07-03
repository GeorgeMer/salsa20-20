#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../salsa_core/salsa_core_v1.h"

void salsa_crypt_v1(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv)
{
    uint32_t in[16];

    // initalize input matrix
    in[0] = 0x61707865;
    in[1] = __builtin_bswap32(key[0]);
    in[2] = __builtin_bswap32(key[1]);
    in[3] = __builtin_bswap32(key[2]);
    in[4] = __builtin_bswap32(key[3]);
    in[5] = 0x3320646e;
    in[6] = __builtin_bswap32(iv >> 32);
    in[7] = __builtin_bswap32(iv & 0xffffffff);
    in[8] = UINT32_MAX;
    in[9] = UINT32_MAX;
    in[10] = 0x79622d32;
    in[11] = __builtin_bswap32(key[4]);
    in[12] = __builtin_bswap32(key[5]);
    in[13] = __builtin_bswap32(key[6]);
    in[14] = __builtin_bswap32(key[7]);
    in[15] = 0x6b206574;

    uint32_t hash[16]; // output array

    uint64_t i = 0;
    // use one hash to encode 64 bytes of msg
    for (; i < mlen / 64; i++)
    {

        // set counter
        if (in[8] == UINT32_MAX && in[9] == UINT32_MAX)
        {
            in[8] = 0;
            in[9] = 0;
        }
        else if (in[8] == UINT32_MAX)
        {
            in[9] += 1;
            in[8] = 0;
        }
        else
        {
            in[8] += 1;
        }

        // get 64byte hash
        salsa_core_v1(hash, in);

        // go byte by byte through hash and msg, xor to cipher
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t k = 0; k < 4; k++)
                cipher[64 * i + 4 * j + k] = ((hash[j] >> 8 * k) & 0xFF) ^ msg[64 * i + 4 * j + k];
    }

    // remaining bytes
    if (in[8] == UINT32_MAX && in[9] == UINT32_MAX)
    {
        in[8] = 0;
        in[9] = 0;
    }
    else if (in[8] == UINT32_MAX)
    {
        in[9] += 1;
        in[8] = 0;
    }
    else
    {
        in[8] += 1;
    }

    // get hash
    salsa_core_v1(hash, in);

    i *= 64;

    // transform it to byte-array
    uint8_t *h = ((uint8_t *)hash);

    // go byte by byte through hash and msg, xor to cipher
    for (uint8_t j = 0; j < 64 && i + j < mlen; j++)
        cipher[i + j] = (h[j]) ^ msg[i + j];
}