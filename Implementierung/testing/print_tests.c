#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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