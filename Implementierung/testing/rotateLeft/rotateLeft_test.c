#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <string.h>
#include <time.h>
#include "../../salsa_core/rotate_left.h"

void print128_num(__m128i var)
{
    uint32_t val[4];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %i %i %i %i \n",
           val[0], val[1], val[2], val[3]);
}

int main()
{

    clock_t start, end;
    double cpu_time_used;

    uint32_t input[] = {0x02, 0x02, 0x22, 0x02};
    uint32_t output[4];

    printf("SISD:\n");

    start = clock();

    for (int i = 0; i < 100000000; i++)
    {
        for (int i = 0; i < 4; i++)
        {
            /* printf("0x%x\n", rotateLeft(input[i], 4)); */
            rotateLeft(input[i], 4);
        }
    }

    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %lf s\n", cpu_time_used);
    printf("--------------------\n");

    printf("SIMD 128:\n");

    __m128i_u _output;

    start = clock();
    for (int i = 0; i < 100000000; i++)
    {
        _output = rotateLeftSIMD_128(
            _mm_set_epi32(input[3], input[2], input[1], input[0]), 4);
    }
    end = clock();

cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %lf s\n", cpu_time_used);
    _mm_storeu_si128((__m128i *)output, _output);

    /* for (int i = 0; i < 4; i++)
        printf("0x%x\n", output[i]); */

    printf("--------------------\n");

    printf("SIMD 32:\n");
    rotateLeftSIMD_32(input, 4);
    for (int i = 0; i < 4; i++)
        printf("0x%x\n", input[i]);

    return EXIT_SUCCESS;
}