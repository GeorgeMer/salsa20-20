#include <stddef.h>
#include <stdint.h>

#include "rotate_left.h"

void columnRound_v2(uint32_t output[16]) 
{

    __m128i numbers0  = _mm_setr_epi32(output[0], output[1], output[2], output[3]);
    __m128i numbers4  = _mm_setr_epi32(output[4], output[5], output[6], output[7]);
    __m128i numbers8  = _mm_setr_epi32(output[8], output[9], output[10], output[11]);
    __m128i numbers12 = _mm_setr_epi32(output[12], output[13], output[14], output[15]);

    // rotating left by 7 bits and xor'ing
    numbers4 = rotateLeftSIMD_128_xor(7, numbers0, numbers12, numbers4);
    
    // rotating left by 9 bits and xor'ing
    numbers4 = rotateLeftSIMD_128_xor(9, numbers0, numbers4, numbers8);

    // rotating left by 13 bits and xor'ing
    numbers12 = rotateLeftSIMD_128_xor(13, numbers8, numbers4, numbers12);

    // rotating left by 18 bits and xor'ing
    numbers0 = rotateLeftSIMD_128_xor(18, numbers12, numbers8, numbers0);

    _mm_storeu_si128((__m128i *)output, numbers0);
    _mm_storeu_si128((__m128i *)output+4, numbers4);
    _mm_storeu_si128((__m128i *)output+8, numbers8);
    _mm_storeu_si128((__m128i *)output+12, numbers12);
}

void rowRound_v2(uint32_t output[16]) 
{
    // rotating left by 7 bits and xor'ing
    rotateLeftSIMD_32_xor(7, 
        output[0] +     output[3], &output[1],
        output[5] +     output[4],  &output[6],
        output[10] +    output[9],  &output[11],
        output[15] +    output[14], &output[12]
    );

    // rotating left by 9 bits and xor'ing
    rotateLeftSIMD_32_xor(9, 
        output[0] +     output[1],  &output[2],
        output[5] +     output[6],  &output[7],
        output[10] +    output[11], &output[8],
        output[15] +    output[12], &output[13]
    );

    // rotating left by 13 bits and xor'ing
    rotateLeftSIMD_32_xor(13, 
        output[2] +    output[1],  &output[3],
        output[7] +   output[6],  &output[4],
        output[8] +    output[11], &output[9],
        output[13] +    output[12],  &output[14]
    );

    // rotating left by 18 bits and xor'ing
    rotateLeftSIMD_32_xor(18, 
        output[3] +   output[2],  &output[0],
        output[4] +    output[7], &output[5],
        output[9] +    output[8],  &output[10],
        output[14] +   output[13],  &output[15]
    );

}

void salsa_core_v2(uint32_t output[16], const uint32_t input[16])
{
    /* for (uint8_t i = 0; i < 16; i++)
    {
        output[i] = input[i];
    } */
    output[0] = input[0];
    output[1] = input[5];
    output[2] = input[10];
    output[3] = input[15];

    output[4] = input[4];
    output[5] = input[9];
    output[6] = input[14];
    output[7] = input[3];

    output[8] = input[8];
    output[9] = input[13];
    output[10] = input[2];
    output[11] = input[7];

    output[12] = input[12];
    output[13] = input[1];
    output[14] = input[6];
    output[15] = input[11];
    

    for (size_t i = 0; i < 20; i++) 
    {
        columnRound_v2(output);
        /* rowRound_v2(output); */

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


    uint32_t temp[16];
    for (int i = 0; i < 16; i++) {
        temp[i] = output[i];
    }

    output[0] = temp[0];
    output[1] = temp[5];
    output[2] = temp[10];
    output[3] = temp[15];
    output[4] = temp[4];
    output[5] = temp[9];
    output[6] = temp[14];
    output[7] = temp[3];
    output[8] = temp[8];
    output[9] = temp[13];
    output[10] = temp[2];
    output[11] = temp[7];
    output[12] = temp[12];
    output[13] = temp[1];
    output[14] = temp[6];
    output[15] = temp[11];

    for (int i = 0; i < 16; i++)
    {
        output[i] += input[i];
    }
}