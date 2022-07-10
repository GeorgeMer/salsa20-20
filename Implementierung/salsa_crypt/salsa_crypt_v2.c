#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>
#include "../salsa_core/salsa_core_v2.h"

void salsa_crypt_v2(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv)
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
    in[8] = 0;
    in[9] = 0;
    in[10] = 0x79622d32;
    in[11] = __builtin_bswap32(key[4]);
    in[12] = __builtin_bswap32(key[5]);
    in[13] = __builtin_bswap32(key[6]);
    in[14] = __builtin_bswap32(key[7]);
    in[15] = 0x6b206574;

    uint32_t out[16];
    salsa_core_v2(out, in);

    size_t i = 0;
    size_t index = 0;

    // uint8_t stream of keystream in little endian order
    const uint8_t *stream = ((uint8_t *)out);

    // xor msg with keystream
    for (i = 0; i < mlen; i += 16)
    {
        if (mlen - i < 16)
        {
            goto non_vectorized;
        }

        __m128i bytes_of_msg = _mm_lddqu_si128((__m128i *)(msg + i));
        __m128i bytes_of_stream = _mm_lddqu_si128(((__m128i *)stream) + index);

        __m128i result = _mm_xor_si128(bytes_of_msg, bytes_of_stream);

        _mm_storeu_si128((__m128i *)(cipher + i), result);

        if (index == 4)
        {
            index = 0;
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

            salsa_core_v2(out, in);
            stream = ((uint8_t *)out);
        }
        index++;
    }

non_vectorized:
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

    salsa_core_v2(out, in);
    stream = ((uint8_t *)out);

    for (size_t j = 0; i < mlen; j++, i++)
    {
        cipher[i] = msg[i] ^ (*(stream + j));
    }
}