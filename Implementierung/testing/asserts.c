#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// assertions used for testing
// they test equality of arrays, first compare length, then compare content.
// if something is wrong, assertion stops and false is returned with an error message printed to console
// all methods are the same apart from the array values

bool assertEqualsArrays_8bit(uint64_t xlen, const uint8_t expected[xlen], uint64_t alen, uint8_t actual[alen])
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %lu != %lu", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %lu \nLenght: %lu\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n\n\n");
    return true;
}

bool assertEqualsArrays_16bit(uint64_t xlen, const uint16_t expected[xlen], uint64_t alen, uint16_t actual[alen])
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %lu != %lu", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %lu \nLenght: %lu\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n\n\n");
    return true;
}

bool assertEqualsArrays_32bit(uint64_t xlen, const uint32_t expected[xlen], uint64_t alen, uint32_t actual[alen])
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %lu != %lu", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %lu \nLenght: %lu\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n\n\n");
    return true;
}

bool assertEqualsArrays_64bit(uint64_t xlen, const uint64_t expected[xlen], uint64_t alen, uint64_t actual[alen])
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %lu != %lu", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %lu \nActual: %lu \nAt: %lu \nLenght: %lu\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n\n\n");
    return true;
}
