#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>


int randint(int n)
{
    if ((n - 1) == RAND_MAX)
    {
        return rand();
    }
    else
    {
        // Supporting larger values for n would requires an even more
        // elaborate implementation that combines multiple calls to rand()
        assert(n <= RAND_MAX);

        // Chop off all of the values that would cause skew...
        int end = RAND_MAX / n; // truncate skew
        assert(end > 0);
        end *= n;

        // ... and ignore results from rand() that fall above that limit.
        // (Worst case the loop condition should succeed 50% of the time,
        // so we can expect to bail out of this loop pretty quickly.)
        int r;
        while ((r = rand()) >= end)
            ;

        return r % n;
    }
}

int randomInt(int from, int to)
{
    return from + randint(to - from);
}

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
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i,xlen);
            return false;
        }
    }
    printf("Success\n");
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
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i,xlen);
             return false;
        }
    }
    printf("Success\n");
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
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i,xlen);
             return false;
        }
    }
    printf("Success\n");
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
            printf("Expected: %u \nActual: %u \nAt: %u \nLenght: %u\n", expected[i], actual[i], i,xlen);
             return false;
        }
    }
    printf("Success\n");
    return true;
}
