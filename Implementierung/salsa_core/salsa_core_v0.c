#include "rotate_left.h"

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