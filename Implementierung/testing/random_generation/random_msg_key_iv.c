#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "random_numbers.h"

// these functions generate random values used for testing

// a random message
void randomMsg(size_t mlen, uint8_t msg[mlen])
{
    for (uint64_t i = 0; i < mlen; i++)
    {
        msg[i] = randomInt(33, 126);
    }
}

// a random key
void randomKey(uint32_t key[8])
{
    for (uint8_t i = 0; i < 8; i++)
    {
        key[i] = 2 * ((uint64_t)randomInt(0, RAND_MAX)) + randomInt(0, 1);
    }
}

// a random nonce
uint64_t randomNonce()
{
    uint64_t iv;
    // randomInt generates only up to 2^32, so concatenation of two such random values
    iv = ((((uint64_t)randomInt(0, RAND_MAX)) * ((uint64_t)randomInt(0, RAND_MAX)) + randomInt(0, 1)) << 32) | (2 * ((uint64_t)randomInt(0, RAND_MAX)) + randomInt(0, 1));
    return iv;
}
