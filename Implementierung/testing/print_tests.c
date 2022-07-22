#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// prints a uint8 array as a string
void print_char_arr(size_t len, const uint8_t arr[len])
{
    for (size_t i = 0; i < len; i++)
        printf("%c", arr[i]);
    printf("\n");
}

// prints uint8 array in hex representation
void print_arr_8bit(size_t len, const uint8_t arr[len])
{
    for (size_t i = 0; i < len; i++)
        printf("%02x", arr[i]);
    printf("\n");
}

// prints uint32 array in hex representation
void print_arr_32bit(size_t len, const uint32_t arr[len])
{
    for (size_t i = 0; i < len; i++)
        printf("%08x", arr[i]);
    printf("\n");
}
