#include <stddef.h>
#include <stdint.h>

#define R(a, b) (((a) << (b)) | ((a) >> (32 - (b))))

//  http://cr.yp.to/salsa20.html
//  by D. J. Bernstein, creator of the Salsa20/20 encryption algorithm

void salsa20_word_specification(uint32_t out[16], uint32_t in[16])
{
    int i;
    uint32_t x[16];
    for (i = 0; i < 16; ++i)
        x[i] = in[i];
    for (i = 20; i > 0; i -= 2)
    {
        x[4] ^= R(x[0] + x[12], 7);
        x[8] ^= R(x[4] + x[0], 9);
        x[12] ^= R(x[8] + x[4], 13);
        x[0] ^= R(x[12] + x[8], 18);
        x[9] ^= R(x[5] + x[1], 7);
        x[13] ^= R(x[9] + x[5], 9);
        x[1] ^= R(x[13] + x[9], 13);
        x[5] ^= R(x[1] + x[13], 18);
        x[14] ^= R(x[10] + x[6], 7);
        x[2] ^= R(x[14] + x[10], 9);
        x[6] ^= R(x[2] + x[14], 13);
        x[10] ^= R(x[6] + x[2], 18);
        x[3] ^= R(x[15] + x[11], 7);
        x[7] ^= R(x[3] + x[15], 9);
        x[11] ^= R(x[7] + x[3], 13);
        x[15] ^= R(x[11] + x[7], 18);
        x[1] ^= R(x[0] + x[3], 7);
        x[2] ^= R(x[1] + x[0], 9);
        x[3] ^= R(x[2] + x[1], 13);
        x[0] ^= R(x[3] + x[2], 18);
        x[6] ^= R(x[5] + x[4], 7);
        x[7] ^= R(x[6] + x[5], 9);
        x[4] ^= R(x[7] + x[6], 13);
        x[5] ^= R(x[4] + x[7], 18);
        x[11] ^= R(x[10] + x[9], 7);
        x[8] ^= R(x[11] + x[10], 9);
        x[9] ^= R(x[8] + x[11], 13);
        x[10] ^= R(x[9] + x[8], 18);
        x[12] ^= R(x[15] + x[14], 7);
        x[13] ^= R(x[12] + x[15], 9);
        x[14] ^= R(x[13] + x[12], 13);
        x[15] ^= R(x[14] + x[13], 18);
    }
    for (i = 0; i < 16; ++i)
        out[i] = x[i] + in[i];
}

void salsa_crypt_reference(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv)
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

    // output matrix
    uint32_t out[16];
    salsa20_word_specification(out, in);

    size_t index = 0;

    // uint8_t stream of keystream in little endian order
    uint8_t *stream = ((uint8_t *)out);

    // xor msg with keystream and increment counter after 64 bytes
    for (size_t i = 0; i < mlen; i++)
    {
        if (index == 64)
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

            salsa20_word_specification(out, in);
            stream = ((uint8_t *)out);
        }
        cipher[i] = msg[i] ^ (*(stream + index));
        index++;
    }
}
