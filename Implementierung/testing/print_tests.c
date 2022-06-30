#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void print_char_arr(const uint8_t arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%c", arr[i]);
    printf("\n");
}

void print_arr_8bit(const uint8_t arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%02x", arr[i]);
    printf("\n");
}

void print_arr_32bit(const uint32_t arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%08x", arr[i]);
    printf("\n");
}

void print_key(const uint32_t arr[])
{
    print_arr_32bit(arr, 8);
}

void print_crypt(const uint32_t arr[])
{
    for (size_t i = 0; i < 16; i++)
    {
        for (size_t j = 0; j < 4; j++)
            printf("%02x ", (arr[i] >> 8 * j) & 0xFF);
        printf("| ");
    }
    printf("\n");
}