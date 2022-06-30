#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void salsa20_crypt(size_t mlen, const uint8_t msg[mlen], uint8_t cipher[mlen], uint32_t key[8], uint64_t iv){
    uint32_t in[16];

    //write to start array, except to 64bits of counter
    in[0] = 0x61707865;
    in[1] = key[0];
    in[2] = key[1];
    in[3] = key[2];
    in[4] = key[3];
    in[5] = 0x3320646e;
    in[6] = iv & 0xFFFFFFFF;
    in[7] = (iv>>32) & 0xFFFFFFFF;

    in[10] = 0x79622d32;
    in[11] = key[4];
    in[12] = key[5];
    in[13] = key[6];
    in[14] = key[7];
    in[15] = 0x6b206574;


    uint32_t hash[16]; //output array

    //use one hash to encode 64 bytes of msg
    for(uint64_t i=0; i<mlen/64; i++){


        //TODO: use simd


        //set counter
        in[8] = i & 0xFFFFFFFF;      
        in[9] = (i>>32) & 0xFFFFFFFF;

        //get 64byte hash
        salsa20_core(hash,in);

        //go byte by byte through hash and msg, xor to cipher
        for(uint8_t j=0;j<16;j++)
            for(uint8_t k=0;k<4;k++)
                cipher[4*j+k] = ((hash[j] >> 8*k) & 0xFF) ^ msg[4*j+k];  
    }

}