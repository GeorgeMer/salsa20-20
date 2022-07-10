#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>

uint32_t rotateLeft(uint32_t number, uint32_t amount);
__m128i rotateLeftSIMD_128(__m128i numbers, uint32_t amount);
void rotateLeftSIMD_32(uint32_t numbers[4], uint32_t amount);
void rotateLeftSIMD_32_xor(uint32_t amount, uint32_t n0, uint32_t *output0, uint32_t n1, uint32_t *output1,
                           uint32_t n2, uint32_t *output2, uint32_t n3, uint32_t *output3);
__m128i rotateLeftSIMD_128_xor(uint32_t amount, __m128i numbers0, __m128i numbers1, __m128i xor);