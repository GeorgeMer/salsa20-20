#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>

uint32_t rotateLeft(uint32_t number, uint32_t amount);
__m128i rotateLeftSIMD_128(__m128i numbers, uint32_t amount);
void rotateLeftSIMD_32(uint32_t numbers[4], uint32_t amount);