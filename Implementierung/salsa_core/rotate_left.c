#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>

uint32_t rotateLeft(uint32_t number, uint32_t amount)
{
    return (number << amount) | (number >> (32 - amount));
}

__m128i rotateLeftSIMD_128(__m128i numbers, uint32_t amount) {
    // input has to be reversed after 
    return _mm_or_si128(
        _mm_slli_epi32(numbers, amount),
        _mm_srli_epi32(numbers, 32 - amount)
    );
}

void rotateLeftSIMD_32(uint32_t numbers[4], uint32_t amount)
{
    // numbers are put into _numbers in reversed order so that they will be returned in the right order
    __m128i_u _numbers = _mm_setr_epi32(numbers[0], numbers[2], numbers[2], numbers[3]);
    
    _numbers = _mm_or_si128(
        _mm_srli_epi32(_numbers, 32 - amount),
        _mm_slli_epi32(_numbers, amount));

    _mm_storeu_si128((__m128i*) numbers, _numbers);
}