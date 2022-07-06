#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "../salsa_crypt/salsa_crypt_v1.h"
#include "../salsa_crypt/salsa_crypt_v2.h"
#include "asserts.h"
#include "print_tests.h"

void salsa20_crypt_test(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv)
{
    uint32_t in[16];

    // write to start array, except to 64bits of counter
    in[0] = 0x61707865;
    in[1] = key[0];
    in[2] = key[1];
    in[3] = key[2];
    in[4] = key[3];
    in[5] = 0x3320646e;
    in[6] = iv & 0xFFFFFFFF;
    in[7] = (iv >> 32) & 0xFFFFFFFF;

    in[10] = 0x79622d32;
    in[11] = key[4];
    in[12] = key[5];
    in[13] = key[6];
    in[14] = key[7];
    in[15] = 0x6b206574;

    uint32_t hash[16]; // output array

    uint64_t i = 0;
    // use one hash to encode 64 bytes of msg
    for (; i < mlen / 64 + 1; i++)
    {
        // set counter
        in[8] = i & 0xFFFFFFFF;
        in[9] = (i >> 32) & 0xFFFFFFFF;

        // get 64byte hash
        salsa20_core(hash, in);

        //transform it to byte-array
        uint8_t h[64];
        for (uint8_t j = 0; j < 16; j++)
        {
            h[j*4] = ((hash[j] >> 8 * 0) & 0xFF);
            h[j*4 + 1] = ((hash[j] >> 8 * 1) & 0xFF);
            h[j*4 + 2] = ((hash[j] >> 8 * 2) & 0xFF);
            h[j*4 + 3] = ((hash[j] >> 8 * 3) & 0xFF);
        }

        // go byte by byte through hash and msg, xor to cipher
        for (uint8_t j = 0; j < 64 && 64 * i + j < mlen; j++)
            cipher[64 * i + j] = (h[j]) ^ msg[64 * i + j];
    }
}

int main(int argc, char **argv)
{

    if (argc != 2 || atoi(argv[1]) <= 0)
    {
        printf("Invalid Arguments\n");
        return EXIT_FAILURE;
    }

    int tests = atoi(argv[1]);

    for (int i = 0; i < tests; i++)
    {
       
 
        srand(i + time(NULL));
        
        uint64_t mlen = randomInt(0, 100000);
        uint8_t msg[mlen];
        uint8_t cipher1[mlen];
        uint8_t cipher2[mlen];
        uint32_t key[8];
        uint64_t iv;

        for (uint64_t i = 0; i < mlen; i++)
        {
            uint8_t cur = randomInt(33, 126);
            msg[i] = cur;
        }
        for (uint8_t i = 0; i < 8; i++)
        {
            key[i] = 2 * ((uint64_t)randomInt(0, RAND_MAX)) + randomInt(0, 1);
        }
        iv = (( ((uint64_t)randomInt(0, RAND_MAX)) * ((uint64_t)randomInt(0, RAND_MAX)) + randomInt(0, 1)) << 32) | (2 * ((uint64_t)randomInt(0, RAND_MAX)) + randomInt(0, 1));
        
     
        salsa_crypt_v1(mlen, msg, cipher1, key, iv);
        
        salsa_crypt_v2(mlen, msg, cipher2, key, iv);
        print_arr_8bit(cipher2, mlen);
        print_char_arr(msg, mlen);
        print_key(key);
        printf("Nonce: %llx\n", iv);

        printf("%d: ",i);
        
        if(! (assertEqualsArrays_8bit(cipher1,mlen,cipher2,mlen))  ){
            return EXIT_FAILURE;
        }
    }

    return 0;
}