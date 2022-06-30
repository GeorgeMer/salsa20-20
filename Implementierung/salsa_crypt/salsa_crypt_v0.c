#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

uint32_t rotateLeft(uint32_t number, uint32_t amount)
{
    return (number << amount) | (number >> (32 - amount));
}

void salsa20_core(uint32_t output[16], const uint32_t input[16])
{
    for (uint8_t i = 0; i < 16; i++)
    {
        output[i] = input[i];
    }

    // 20 rounds
    for (size_t i = 0; i < 20; i++)
    {
        // rotating left by 7 bits
        output[4] = (rotateLeft((output[0] + output[12]), 7)) ^ output[4];
        output[9] = (rotateLeft((output[5] + output[1]), 7)) ^ output[9];
        output[14] = (rotateLeft((output[10] + output[6]), 7)) ^ output[14];
        output[3] = (rotateLeft((output[15] + output[11]), 7)) ^ output[3];

        // rotating left by 9 bits
        output[8] = (rotateLeft((output[0] + output[4]), 9)) ^ output[8];
        output[13] = (rotateLeft((output[5] + output[9]), 9)) ^ output[13];
        output[2] = (rotateLeft((output[10] + output[14]), 9)) ^ output[2];
        output[7] = (rotateLeft((output[15] + output[3]), 9)) ^ output[7];

        // rotating left by 13 bits
        output[12] = (rotateLeft((output[8] + output[4]), 13)) ^ output[12];
        output[1] = (rotateLeft((output[13] + output[9]), 13)) ^ output[1];
        output[6] = (rotateLeft((output[2] + output[14]), 13)) ^ output[6];
        output[11] = (rotateLeft((output[7] + output[3]), 13)) ^ output[11];

        // rotating left by 18 bits
        output[0] = (rotateLeft((output[12] + output[8]), 18)) ^ output[0];
        output[5] = (rotateLeft((output[1] + output[13]), 18)) ^ output[5];
        output[10] = (rotateLeft((output[6] + output[2]), 18)) ^ output[10];
        output[15] = (rotateLeft((output[11] + output[7]), 18)) ^ output[15];

        // transpose matrix
        uint32_t tmp;
        tmp = output[1];
        output[1] = output[4];
        output[4] = tmp;

        tmp = output[2];
        output[2] = output[8];
        output[8] = tmp;

        tmp = output[3];
        output[3] = output[12];
        output[12] = tmp;

        tmp = output[6];
        output[6] = output[9];
        output[9] = tmp;

        tmp = output[7];
        output[7] = output[13];
        output[13] = tmp;

        tmp = output[11];
        output[11] = output[14];
        output[14] = tmp;
    }

    for (int i = 0; i < 16; i++)
    {
        output[i] += input[i];
    }
}

void salsa20_crypt(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv)
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
    for (; i < mlen / 64 + 1; i++)
    {
        // set counter
        in[8] = i & 0xFFFFFFFF;
        in[9] = (i >> 32) & 0xFFFFFFFF;

        // get 64byte hash
        salsa20_core(hash, in);

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
        for (uint8_t j = 0; j < 64 && 64 * i + j < mlen; j++)
            cipher[64 * i + j] = (h[j]) ^ msg[64 * i + j];
    }
}
