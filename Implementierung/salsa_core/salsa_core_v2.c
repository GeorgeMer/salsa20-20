#include "rotate_left.h"

// this implementation works like v1 but with SIMD instructions

void columnRound_v2(uint32_t output[16])
{
    // a quarterround with SIMD instructions
    rotateLeftSIMD_32_xor(7,
                          output[0] + output[12], &output[4],
                          output[5] + output[1], &output[9],
                          output[10] + output[6], &output[14],
                          output[15] + output[11], &output[3]);

    rotateLeftSIMD_32_xor(9,
                          output[0] + output[4], &output[8],
                          output[5] + output[9], &output[13],
                          output[10] + output[14], &output[2],
                          output[15] + output[3], &output[7]);

    rotateLeftSIMD_32_xor(13,
                          output[8] + output[4], &output[12],
                          output[13] + output[9], &output[1],
                          output[2] + output[14], &output[6],
                          output[7] + output[3], &output[11]);

    rotateLeftSIMD_32_xor(18,
                          output[12] + output[8], &output[0],
                          output[1] + output[13], &output[5],
                          output[6] + output[2], &output[10],
                          output[11] + output[7], &output[15]);
}

void rowRound_v2(uint32_t output[16])
{
    rotateLeftSIMD_32_xor(7,
                          output[0] + output[3], &output[1],
                          output[5] + output[4], &output[6],
                          output[10] + output[9], &output[11],
                          output[15] + output[14], &output[12]);

    rotateLeftSIMD_32_xor(9,
                          output[0] + output[1], &output[2],
                          output[5] + output[6], &output[7],
                          output[10] + output[11], &output[8],
                          output[15] + output[12], &output[13]);

    rotateLeftSIMD_32_xor(13,
                          output[2] + output[1], &output[3],
                          output[7] + output[6], &output[4],
                          output[8] + output[11], &output[9],
                          output[13] + output[12], &output[14]);

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

    // 10 doublerounds
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