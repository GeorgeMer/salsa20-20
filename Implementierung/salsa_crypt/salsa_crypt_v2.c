#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>
#include "../salsa_core/salsa_core_v2.h"

void salsa20_crypt_v2(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv)
{
    uint32_t in[16];

    // write to start array, except to 64bits of counter
    in[0] = 0x61707865;
    in[1] = key[0];
    in[2] = key[1];
    in[3] = key[2];
    in[4] = key[3];
    in[5] = 0x3320646e;
    in[6] = iv & 0xFFFFFFFF;
    in[7] = (iv >> 32) & 0xFFFFFFFF;

    in[10] = 0x79622d32;
    in[11] = key[4];
    in[12] = key[5];
    in[13] = key[6];
    in[14] = key[7];
    in[15] = 0x6b206574;

    uint32_t hash[16]; // output array
    uint64_t i = 0;
    // use one hash to encode 64 bytes of msg
    for (; i < mlen / 64; i++)
    {

        // set counter
        in[8] = i & 0xFFFFFFFF;
        in[9] = (i >> 32) & 0xFFFFFFFF;

        // get 64byte hash
        salsa_core_v2(hash, in);

        for (uint8_t j = 0; j < 4; j++)
        {

            // load 16 bytes of hash
            __m128i h = _mm_loadu_si128(hash + 4 * j);

            // load hash
            __m128i m = _mm_loadu_si128(msg + 64 * i + 16 * j);

            // perform xor
            __m128i c = _mm_xor_si128(h, m);

            // store
            _mm_storeu_si128(cipher + 64 * i + 16 * j, c);
        }
    }

    // -- finish up --

    // set counter
    in[8] = i & 0xFFFFFFFF;
    in[9] = (i >> 32) & 0xFFFFFFFF;

    // get 64byte hash
    salsa_core_v2(hash, in);

    i *= 64;

    // transform it to byte-array
    uint8_t h[64];
    for (uint8_t j = 0; j < 16; j++)
    {
        h[j * 4] = ((hash[j] >> 8 * 0) & 0xFF);
        h[j * 4 + 1] = ((hash[j] >> 8 * 1) & 0xFF);
        h[j * 4 + 2] = ((hash[j] >> 8 * 2) & 0xFF);
        h[j * 4 + 3] = ((hash[j] >> 8 * 3) & 0xFF);
    }

    // go byte by byte through hash and msg, xor to cipher
    for (uint8_t j = 0; j < 64 && i + j < mlen; j++)
        cipher[i + j] = (h[j]) ^ msg[i + j];
}