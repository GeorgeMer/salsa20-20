#include "rotate_left.h"

uint32_t rotateLeft(uint32_t number, uint32_t amount)
{
    return (number << amount) | (number >> (32 - amount));
}