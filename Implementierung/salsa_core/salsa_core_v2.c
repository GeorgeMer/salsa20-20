#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int rotateLeft(uint32_t n, uint32_t d)
{
    return (n << d) | (n >> (32 - d));
}

void quarterRound(uint32_t output[4], const uint32_t input[4])
{
    output[1] = input[1] ^ (rotateLeft(input[0] + input[3], 7));
    output[2] = input[2] ^ (rotateLeft(input[1] + input[0], 9));
    output[3] = input[3] ^ (rotateLeft(input[2] + input[1], 13));
    output[0] = input[0] ^ (rotateLeft(input[3] + input[2], 18));
}

void rowRound(uint32_t output[16], const uint32_t input[16])
{
    uint32_t temp_in[4];
    uint32_t temp_out[4];

    temp_in[0] = input[0];
    temp_in[1] = input[1];
    temp_in[2] = input[2];
    temp_in[3] = input[3];
    quarterRound(temp_out, temp_in);
    output[0] = temp_out[0];
    output[1] = temp_out[1];
    output[2] = temp_out[2];
    output[3] = temp_out[3];

    temp_in[0] = input[5];
    temp_in[1] = input[6];
    temp_in[2] = input[7];
    temp_in[3] = input[4];
    quarterRound(temp_out, temp_in);
    output[5] = temp_out[0];
    output[6] = temp_out[1];
    output[7] = temp_out[2];
    output[4] = temp_out[3];

    temp_in[0] = input[10];
    temp_in[1] = input[11];
    temp_in[2] = input[8];
    temp_in[3] = input[9];
    quarterRound(temp_out, temp_in);
    output[10] = temp_out[0];
    output[11] = temp_out[1];
    output[8] = temp_out[2];
    output[9] = temp_out[3];

    temp_in[0] = input[15];
    temp_in[1] = input[12];
    temp_in[2] = input[13];
    temp_in[3] = input[14];
    quarterRound(temp_out, temp_in);
    output[15] = temp_out[0];
    output[12] = temp_out[1];
    output[13] = temp_out[2];
    output[14] = temp_out[3];
}

void columnRound(uint32_t output[16], const uint32_t input[16])
{
    uint32_t temp_in[4];
    uint32_t temp_out[4];

    temp_in[0] = input[0];
    temp_in[1] = input[4];
    temp_in[2] = input[8];
    temp_in[3] = input[12];
    quarterRound(temp_out, temp_in);
    output[0] = temp_out[0];
    output[4] = temp_out[1];
    output[8] = temp_out[2];
    output[12] = temp_out[3];

    temp_in[0] = input[5];
    temp_in[1] = input[9];
    temp_in[2] = input[13];
    temp_in[3] = input[1];
    quarterRound(temp_out, temp_in);
    output[5] = temp_out[0];
    output[9] = temp_out[1];
    output[13] = temp_out[2];
    output[1] = temp_out[3];

    temp_in[0] = input[10];
    temp_in[1] = input[14];
    temp_in[2] = input[2];
    temp_in[3] = input[6];
    quarterRound(temp_out, temp_in);
    output[10] = temp_out[0];
    output[14] = temp_out[1];
    output[2] = temp_out[2];
    output[6] = temp_out[3];

    temp_in[0] = input[15];
    temp_in[1] = input[3];
    temp_in[2] = input[7];
    temp_in[3] = input[11];
    quarterRound(temp_out, temp_in);
    output[15] = temp_out[0];
    output[3] = temp_out[1];
    output[7] = temp_out[2];
    output[11] = temp_out[3];
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

uint8_t salsa20_hash(uint8_t output[64], const uint8_t input[64])
{
    uint8_t temp_input[4];
    uint32_t temp_output[16];
    for (int i = 0, j = 0; i < 16; i += 4, j++)
    {
        temp_input[0] = input[i];
        temp_input[1] = input[i + 1];
        temp_input[2] = input[i + 2];
        temp_input[3] = input[i + 3];
        temp_output[j] = littleendian(temp_input);
    }

    uint32_t temp_output0[16];
    for (int i = 0; i < 10; i++) {
        
    }
}

void salsa20_core(uint32_t output[16], const uint32_t input[16])
{
    for (size_t i = 0; i < 20; i++)
    {
    }
}
