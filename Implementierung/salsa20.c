#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int rotateLeft(uint32_t n, uint32_t d) {
   return (n << d)|(n >> (32 - d));
}

void quarterRound(uint32_t output[4], const uint32_t input[4]) {
    output[1] = input[1] ^ (rotateLeft(input[0] + input[3], 7));
    output[2] = input[2] ^ (rotateLeft(input[1] + input[0], 9));
    output[3] = input[3] ^ (rotateLeft(input[2] + input[1], 13));
    output[0] = input[0] ^ (rotateLeft(input[3] + input[2], 18));
}

void rowRound(uint32_t output[16], const uint32_t input[16]){
    uint32_t temp_in[4];
    uint32_t temp_out[4];

    temp_in[0] = input[0];
    temp_in[1] = input[1];
    temp_in[2] = input[2];
    temp_in[3] = input[3];
    quarterRound(temp_out, temp_in);
    output[0] = temp_out[0];
    output[1] = temp_out[1];
    output[2] = temp_out[2];
    output[3] = temp_out[3];

    temp_in[0] = input[5];
    temp_in[1] = input[6];
    temp_in[2] = input[7];
    temp_in[3] = input[4];
    quarterRound(temp_out, temp_in);
    output[5] = temp_out[0];
    output[6] = temp_out[1];
    output[7] = temp_out[2];
    output[4] = temp_out[3];

    temp_in[0] = input[10];
    temp_in[1] = input[11];
    temp_in[2] = input[8];
    temp_in[3] = input[9];
    quarterRound(temp_out, temp_in);
    output[10] = temp_out[0];
    output[11] = temp_out[1];
    output[8] = temp_out[2];
    output[9] = temp_out[3];

    temp_in[0] = input[15];
    temp_in[1] = input[12];
    temp_in[2] = input[13];
    temp_in[3] = input[14];
    quarterRound(temp_out, temp_in);
    output[15] = temp_out[0];
    output[12] = temp_out[1];
    output[13] = temp_out[2];
    output[14] = temp_out[3];
}

void columnRound(uint32_t output[16], const uint32_t input[16]){
    uint32_t temp_in[4];
    uint32_t temp_out[4];

    temp_in[0] = input[0];
    temp_in[1] = input[4];
    temp_in[2] = input[8];
    temp_in[3] = input[12];
    quarterRound(temp_out, temp_in);
    output[0] = temp_out[0];
    output[4] = temp_out[1];
    output[8] = temp_out[2];
    output[12] = temp_out[3];

    temp_in[0] = input[5];
    temp_in[1] = input[9];
    temp_in[2] = input[13];
    temp_in[3] = input[1];
    quarterRound(temp_out, temp_in);
    output[5] = temp_out[0];
    output[9] = temp_out[1];
    output[13] = temp_out[2];
    output[1] = temp_out[3];

    temp_in[0] = input[10];
    temp_in[1] = input[14];
    temp_in[2] = input[2];
    temp_in[3] = input[6];
    quarterRound(temp_out, temp_in);
    output[10] = temp_out[0];
    output[14] = temp_out[1];
    output[2] = temp_out[2];
    output[6] = temp_out[3];

    temp_in[0] = input[15];
    temp_in[1] = input[3];
    temp_in[2] = input[7];
    temp_in[3] = input[11];
    quarterRound(temp_out, temp_in);
    output[15] = temp_out[0];
    output[3] = temp_out[1];
    output[7] = temp_out[2];
    output[11] = temp_out[3];
}

void doubleRound(uint32_t output[16], const uint32_t input[16]) {
    // doubleround(x) = rowround(columnround(x))
    uint32_t temp_output [16];
    columnRound(temp_output, input);
    rowRound(output, temp_output);
}

void salsa20_core(uint32_t output[16], const uint32_t input[16]){
    for (size_t i = 0; i < 20; i++) {

    }
}

void printarr(const uint8_t arr[], size_t len){
    for(size_t i=0;i<len;i++)
        printf("%x ",arr[i]);
    printf("\n");
}

void print_crypt(const uint32_t arr[]){
    for(size_t i=0;i<16;i++){
        for(size_t j=0;j<4;j++)
            printf("%x ",(arr[i] >> 8*j) & 0xFF);
        printf("| ");
    }
    printf("\n");
}

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