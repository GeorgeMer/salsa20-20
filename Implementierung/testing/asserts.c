#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool assertEqualsArrays_8bit(uint64_t xlen, uint8_t expected[xlen], uint64_t alen, uint8_t actual[alen])
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
    printf("Arrays match\n");
    return true;
}

bool assertEqualsArrays_16bit(uint64_t xlen, uint16_t expected[xlen], uint64_t alen, uint16_t actual[alen])
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
    printf("Arrays match\n");
    return true;
}

bool assertEqualsArrays_32bit(uint64_t xlen, uint32_t expected[xlen], uint64_t alen, uint32_t actual[alen])
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
    printf("Arrays match\n");
    return true;
}

bool assertEqualsArrays_64bit(uint64_t xlen, uint64_t expected[xlen], uint64_t alen, uint64_t actual[alen])
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
    printf("Arrays match\n");
    return true;
}
