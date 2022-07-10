#include <stddef.h>
#include <stdint.h>

#include "rotate_left.h"

void columnRound_v1(uint32_t output[16])
{
    // rotating left by 7 bits and xor'ing
    output[4] = (rotateLeft((output[0] + output[12]), 7)) ^ output[4];
    output[9] = (rotateLeft((output[5] + output[1]), 7)) ^ output[9];
    output[14] = (rotateLeft((output[10] + output[6]), 7)) ^ output[14];
    output[3] = (rotateLeft((output[15] + output[11]), 7)) ^ output[3];

    // rotating left by 9 bits and xor'ing
    output[8] = (rotateLeft((output[0] + output[4]), 9)) ^ output[8];
    output[13] = (rotateLeft((output[5] + output[9]), 9)) ^ output[13];
    output[2] = (rotateLeft((output[10] + output[14]), 9)) ^ output[2];
    output[7] = (rotateLeft((output[15] + output[3]), 9)) ^ output[7];

    // rotating left by 13 bits and xor'ing
    output[12] = (rotateLeft((output[8] + output[4]), 13)) ^ output[12];
    output[1] = (rotateLeft((output[13] + output[9]), 13)) ^ output[1];
    output[6] = (rotateLeft((output[2] + output[14]), 13)) ^ output[6];
    output[11] = (rotateLeft((output[7] + output[3]), 13)) ^ output[11];

    // rotating left by 18 bits and xor'ing
    output[0] = (rotateLeft((output[12] + output[8]), 18)) ^ output[0];
    output[5] = (rotateLeft((output[1] + output[13]), 18)) ^ output[5];
    output[10] = (rotateLeft((output[6] + output[2]), 18)) ^ output[10];
    output[15] = (rotateLeft((output[11] + output[7]), 18)) ^ output[15];
}

void rowRound_v1(uint32_t output[16]) 
{
    // rotating left by 7 bits and xor'ing
    output[1] = (rotateLeft((output[0] +  output[3]), 7)) ^ output[1];
    output[6] = (rotateLeft((output[5] +  output[4]), 7)) ^ output[6];
    output[11] = (rotateLeft((output[10] + output[9]), 7)) ^ output[11];
    output[12] = (rotateLeft((output[15] + output[14]), 7)) ^ output[12];

    // rotating left by 9 bits and xor'ing
    output[2] = (rotateLeft(output[0] +  output[1], 9)) ^ output[2];
    output[7] = (rotateLeft(output[5] +  output[6], 9)) ^ output[7];
    output[8] = (rotateLeft(output[10] + output[11], 9)) ^ output[8];
    output[13] = (rotateLeft(output[15] + output[12], 9)) ^ output[13];

    // rotating left by 13 bits and xor'ing
    output[3] = (rotateLeft(output[2] +  output[1], 13)) ^ output[3];
    output[4] = (rotateLeft(output[7] +  output[6], 13)) ^ output[4];
    output[9] = (rotateLeft(output[8] +  output[11], 13)) ^ output[9];
    output[14] = (rotateLeft(output[13] + output[12], 13)) ^ output[14];

    // rotating left by 18 bits and xor'ing
    output[0] = (rotateLeft(output[3] +  output[2], 18)) ^ output[0];
    output[5] = (rotateLeft(output[4] +  output[7], 18)) ^ output[5];
    output[10] = (rotateLeft(output[9] +  output[8], 18)) ^ output[10];
    output[15] = (rotateLeft(output[14] +  output[13], 18)) ^ output[15];

}

void salsa_core_v1(uint32_t output[16], const uint32_t input[16])
{
    for (uint8_t i = 0; i < 16; i++)
    {
        output[i] = input[i];
    }

    // this implementation executes 10 double rounds instead
    // of doing 20 rounds and transposing the matrix everytime 
    for (size_t i = 0; i < 10; i++)
    {
        columnRound_v1(output);
        rowRound_v1(output);
    }

    for (int i = 0; i < 16; i++)
    {
        output[i] += input[i];
    }
}