#include "rotate_left.h"

void columnRound_v2(uint32_t output[16])
{
    // rotating left by 7 bits and xor'ing
    rotateLeftSIMD_32_xor(7,
                          output[0] + output[12], &output[4],
                          output[5] + output[1], &output[9],
                          output[10] + output[6], &output[14],
                          output[15] + output[11], &output[3]);

    // rotating left by 9 bits and xor'ing
    rotateLeftSIMD_32_xor(9,
                          output[0] + output[4], &output[8],
                          output[5] + output[9], &output[13],
                          output[10] + output[14], &output[2],
                          output[15] + output[3], &output[7]);

    // rotating left by 13 bits and xor'ing
    rotateLeftSIMD_32_xor(13,
                          output[8] + output[4], &output[12],
                          output[13] + output[9], &output[1],
                          output[2] + output[14], &output[6],
                          output[7] + output[3], &output[11]);

    // rotating left by 18 bits and xor'ing
    rotateLeftSIMD_32_xor(18,
                          output[12] + output[8], &output[0],
                          output[1] + output[13], &output[5],
                          output[6] + output[2], &output[10],
                          output[11] + output[7], &output[15]);
}

void rowRound_v2(uint32_t output[16])
{
    // rotating left by 7 bits and xor'ing
    rotateLeftSIMD_32_xor(7,
                          output[0] + output[3], &output[1],
                          output[5] + output[4], &output[6],
                          output[10] + output[9], &output[11],
                          output[15] + output[14], &output[12]);

    // rotating left by 9 bits and xor'ing
    rotateLeftSIMD_32_xor(9,
                          output[0] + output[1], &output[2],
                          output[5] + output[6], &output[7],
                          output[10] + output[11], &output[8],
                          output[15] + output[12], &output[13]);

    // rotating left by 13 bits and xor'ing
    rotateLeftSIMD_32_xor(13,
                          output[2] + output[1], &output[3],
                          output[7] + output[6], &output[4],
                          output[8] + output[11], &output[9],
                          output[13] + output[12], &output[14]);

    // rotating left by 18 bits and xor'ing
    rotateLeftSIMD_32_xor(18,
                          output[3] + output[2], &output[0],
                          output[4] + output[7], &output[5],
                          output[9] + output[8], &output[10],
                          output[14] + output[13], &output[15]);
}

void salsa20_core_v2(uint32_t output[16], const uint32_t input[16])
{
    for (uint8_t i = 0; i < 16; i++)
    {
        output[i] = input[i];
    }

    // this implementation executes 10 double rounds instead
    // of doing 20 rounds and transposing the matrix everytime
    for (size_t i = 0; i < 10; i++)
    {
        columnRound_v2(output);
        rowRound_v2(output);
    }

    for (int i = 0; i < 16; i++)
    {
        output[i] += input[i];
    }
}