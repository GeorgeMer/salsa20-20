#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "../salsa_crypt/salsa_crypt_v1.h"
#include "../salsa_crypt/salsa_crypt_v2.h"
#include "asserts.h"


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
        
        uint64_t mlen = randomInt(10, 50000);
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
            key[i] = 2 * randomInt(0, RAND_MAX) + randomInt(0, 1);
        }
        iv = ((2 * randomInt(0, RAND_MAX) + randomInt(0, 1)) << 32) | (2 * randomInt(0, RAND_MAX) + randomInt(0, 1));

     
        salsa_crypt_v1(mlen, msg, cipher1, key, iv);
        
        salsa_crypt_v2(mlen, msg, cipher2, key, iv);
        
        printf("%d: ",i);
        assertEqualsArrays_8bit(cipher1,mlen,cipher2,mlen);
        //if(! ()  ){
        //    return EXIT_FAILURE;
        //}
    }

    return 0;
}