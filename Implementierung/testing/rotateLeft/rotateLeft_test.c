#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <string.h>
#include "../../salsa_core/rotate_left.h"

void print128_num(__m128i var)
{
    uint32_t val[8];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %i %i %i %i \n",
           val[0], val[1], val[2], val[3]);
}

int main()
{
    uint32_t input[] = {0x02, 0x02, 0x22, 0x02};
    uint32_t output[4];


    printf("SISD:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("0x%x\n", rotateLeft(input[i], 4));
    }

    printf("--------------------\n");

    printf("SIMD 128:\n");
    __m128i_u _output = rotateLeftSIMD_128(
        _mm_set_epi32(input[3], input[2], input[1], input[0]), 4);
    /* print128_num(_output); */

    _mm_storeu_si128((__m128i*) output, _output);

    for (int i = 0; i < 4; i++)
        printf("0x%x\n", output[i]);

    printf("--------------------\n");

    printf("SIMD 32:\n");
    rotateLeftSIMD_32(input, 4);
    for (int i = 0; i < 4; i++)
        printf("0x%x\n", input[i]);

    return EXIT_SUCCESS;
}