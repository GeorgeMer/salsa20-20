#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

bool assertEqualsArrays_8bit(uint8_t expected[], uint64_t xlen, uint8_t actual[], uint64_t alen)
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %u != %u", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n");
    return true;
}

bool assertEqualsArrays_16bit(uint16_t expected[], uint64_t xlen, uint16_t actual[], uint64_t alen)
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %u != %u", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n");
    return true;
}

bool assertEqualsArrays_32bit(uint32_t expected[], uint64_t xlen, uint32_t actual[], uint64_t alen)
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %u != %u", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n");
    return true;
}

bool assertEqualsArrays_64bit(uint64_t expected[], uint64_t xlen, uint64_t actual[], uint64_t alen)
{
    if (xlen != alen)
    {
        printf("Lengths don't match: %u != %u", xlen, alen);
        return false;
    }

    for (uint64_t i = 0; i < xlen; i++)
    {
        if (expected[i] != actual[i])
        {
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i, xlen);
            return false;
        }
    }
    printf("Arrays match\n");
    return true;
}
