#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void print_char_arr(size_t len, const uint8_t arr[len])
{
    for (size_t i = 0; i < len; i++)
        printf("%c", arr[i]);
    printf("\n");
}

void print_arr_8bit(size_t len, const uint8_t arr[len])
{
    for (size_t i = 0; i < len; i++)
        printf("%02x", arr[i]);
    printf("\n");
}

void print_arr_32bit(size_t len, const uint32_t arr[len])
{
    for (size_t i = 0; i < len; i++)
        printf("%08x", arr[i]);
    printf("\n");
}

void print_crypt(const uint32_t arr[16])
{
    for (size_t i = 0; i < 16; i++)
    {
        for (size_t j = 0; j < 4; j++)
            printf("%02x ", (arr[i] >> 8 * j) & 0xFF);
        printf("| ");
    }
    printf("\n");
}