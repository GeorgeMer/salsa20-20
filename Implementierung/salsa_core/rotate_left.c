#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>

// rotateLeft as defined by Daniel J. Bernstein
uint32_t rotateLeft(uint32_t number, uint32_t amount)
{
    return (number << amount) | (number >> (32 - amount));
}

// same as function above, but using SIMD and also xor'ing
void rotateLeftSIMD_32_xor(uint32_t amount, uint32_t n0, uint32_t *output0, uint32_t n1, uint32_t *output1,
                           uint32_t n2, uint32_t *output2, uint32_t n3, uint32_t *output3)
{
    // numbers are put into numbers in reversed order so that they will be returned in the right order
    __m128i_u numbers = _mm_setr_epi32(n0, n1, n2, n3);
    __m128i_u xor = _mm_setr_epi32(*output0, *output1, *output2, *output3);

    numbers = _mm_or_si128(
        _mm_srli_epi32(numbers, 32 - amount),
        _mm_slli_epi32(numbers, amount));

    numbers = _mm_xor_si128(numbers, xor);

    uint32_t temp[4];

    _mm_storeu_si128((__m128i *)temp, numbers);

    *output0 = temp[0];
    *output1 = temp[1];
    *output2 = temp[2];
    *output3 = temp[3];
}