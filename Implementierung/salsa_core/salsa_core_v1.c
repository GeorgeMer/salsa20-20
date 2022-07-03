#include <stddef.h>
#include <stdint.h>

#include "rotate_left.h"

void intToBytes(uint8_t bytes[4], uint32_t n)
{
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

void intToBytesP(uint8_t *p, uint32_t n)
{
    p[0] = (n >> 24) & 0xFF;
    p[1] = (n >> 16) & 0xFF;
    p[2] = (n >> 8) & 0xFF;
    p[3] = n & 0xFF;
}

void bytesToInt(uint32_t *p, uint8_t *bytes)
{
    uint32_t out = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}

/* void quarterRound(uint32_t output[4], const uint32_t input[4]) */
void quarterRound(uint32_t *out0, uint32_t *out1, uint32_t *out2, uint32_t *out3,
                  uint32_t in0, uint32_t in1, uint32_t in2, uint32_t in3)
{
    *out1 = in1 ^ (rotateLeft(in0 + in3, 7));
    *out2 = in2 ^ (rotateLeft(in1 + in0, 9));
    *out3 = in3 ^ (rotateLeft(in2 + in2, 13));
    *out0 = in0 ^ (rotateLeft(in3 + in3, 18));
}

void rowRound(uint32_t output[16], const uint32_t input[16])
{
    uint32_t temp_in[4];
    uint32_t temp_out[4];

    quarterRound(&output[0], &output[1], &output[2], &output[3],
                 input[0], input[1], input[2], input[3]);

    quarterRound(&output[5], &output[6], &output[7], &output[4],
                 input[5], input[6], input[7], input[4]);

    quarterRound(&output[10], &output[11], &output[8], &output[9],
                 input[10], input[11], input[8], input[9]);

    quarterRound(&output[15], &output[12], &output[13], &output[14],
                 input[15], input[12], input[13], input[14]);
}

void columnRound(uint32_t output[16], const uint32_t input[16])
{
    uint32_t temp_in[4];
    uint32_t temp_out[4];

    quarterRound(&output[0], &output[4], &output[8], &output[12],
    input[0], input[4], input[8], input[12]);

    quarterRound(&output[5], &output[9], &output[13], &output[1],
    input[5], input[9], input[13], input[1]);

    quarterRound(&output[10], &output[14], &output[2], &output[6],
    input[10], input[14], input[2], input[6]);

    quarterRound(&output[15], &output[3], &output[7], &output[11],
    input[15], input[3], input[7], input[11]);
}

void doubleRound(uint32_t output[16], const uint32_t input[16])
{
    // doubleround(x) = rowround(columnround(x))
    uint32_t temp_output[16];
    columnRound(temp_output, input);
    rowRound(output, temp_output);
}

uint32_t littleendian(uint8_t input[4])
{
    return input[0] +
           input[1] * 256 +
           input[2] * 65536 +
           input[3] * 16777216;
}

void salsa20_hash(uint8_t output[64], const uint8_t input[64])
{
    uint8_t temp_input[4];
    uint32_t x[16];
    for (int i = 0, j = 0; i < 16; i += 4, j++)
    {
        temp_input[0] = input[i];
        temp_input[1] = input[i + 1];
        temp_input[2] = input[i + 2];
        temp_input[3] = input[i + 3];
        x[j] = littleendian(temp_input);
    }

    uint32_t z[16];

    for (int i = 0; i < 16; i++)
    {
        z[i] = x[i];
    }

    uint32_t temp_output[16];

    for (int i = 0; i < 5; i++)
    {
        doubleRound(temp_output, z);
        doubleRound(z, temp_output);
    }

    for (int i = 0; i < 16; i++)
    {
        uint8_t bytes[4];
        intToBytes(bytes, x[i] + z[i]);
        temp_output[i] = littleendian(bytes);
        intToBytesP(&output[i * 4], littleendian(bytes));
    }
}

void salsa20_expansion(uint8_t output[64], uint8_t k[32], int8_t n[16])
{
    uint8_t temp_input[64];

    const uint8_t a_0[4] = {101, 120, 112, 97};
    const uint8_t a_1[4] = {110, 100, 32, 51};
    const uint8_t a_2[4] = {50, 45, 98, 121};
    const uint8_t a_3[4] = {116, 101, 32, 107};

    // (a_0, k_0, a_1, n, a_2, k_1, a_3)
    for (int i = 0; i < 4; i++)
    {
        temp_input[i] = a_0[i];
        temp_input[i + 20] = a_1[i];
        temp_input[i + 40] = a_2[i];
        temp_input[i + 60] = a_3[i];
    }

    for (int i = 0; i < 16; i++)
    {
        temp_input[i + 4] = k[i];
        temp_input[i + 24] = n[i];
        temp_input[i + 44] = k[i + 16];
    }

    salsa20_hash(output, temp_input);
}

void salsa_core_v1(uint32_t output[16], const uint32_t input[16])
{
    // k: input[1-4] and input[11-14]
    uint8_t k[32];
    for (int i = 0; i < 4; i++)
    {
        intToBytesP(&k[i * 4], input[i + 1]);
        intToBytesP(&k[i * 4 + 16], input[i + 11]);
    }

    // nonce = input[6], input[7]
    uint8_t n[16];
    intToBytesP(&n[0], input[6]);
    intToBytesP(&n[4], input[6]);
    // the upper 8 bytes of n remain as 0s

    salsa20_expansion(output, k, n);
}
