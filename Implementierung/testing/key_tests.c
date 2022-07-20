#include <stdbool.h>
#include "../testing/print_tests.h"
#include "../testing/asserts.h"
#include "../number_conversions/number_conversions.h"

bool run_test(const char *key, uint32_t expected[8], uint32_t actual[8])
{
    printf("Key to test: ");
    for (size_t i = 0; i < strlen(key); i++)
    {
        printf("%c", key[i]);
    }
    printf("\n");
    convert_string_to_uint32_t_array(key, actual);
    return assertEqualsArrays_32bit(8, expected, 8, actual);
}

bool compareHexWithString(const char *hex, const char *str)
{
    uint32_t hexarr[8], strarr[8];
    printf("Keys to compare:\n");
    printf("Key hex: ");
    for (size_t i = 0; i < strlen(hex); i++)
    {
        printf("%c", hex[i]);
    }
    printf("\n");
    convert_string_to_uint32_t_array(hex, hexarr);
    printf("\nKey string: ");
    for (size_t i = 0; i < strlen(str); i++)
    {
        printf("%c", str[i]);
    }
    printf("\n");
    convert_string_to_uint32_t_array(str, strarr);
    return assertEqualsArrays_32bit(8, hexarr, 8, strarr);
}

void test_keys()
{

    uint32_t actual[8];

    char *key_shorterThan32 = "A short key";

    uint32_t exp_shorterThan32[] = {
        0x206B6579, 0x686F7274, 0x412073, 0, 0, 0, 0, 0};

    char *key_exactly32 = "This key is 30 plus 2 chars long";

    uint32_t exp_exactly32[] = {
        0x6C6F6E67, 0x61727320, 0x32206368, 0x6C757320, 0x33302070, 0x20697320, 0x206B6579, 0x54686973};

    char *key_moreThan32 = "This key is a lot longer than just the measly 32 bytes";

    uint32_t exp_moreThan32[] = {
        0x79746573, 0x33322062, 0x736C7920, 0x206D6561, 0x20746865, 0x6A757374, 0x68616E20, 0x65722074};

    char *key_hexShorterThan4 = "0x1a3e79c";

    uint32_t exp_hexShorterThan4[] = {
        0x1a3e79c, 0, 0, 0, 0, 0, 0, 0};

    char *key_hexExactly4 = "0x14fb7e92";

    uint32_t exp_hexExactly4[] = {
        0x14fb7e92, 0, 0, 0, 0, 0, 0, 0};

    char *key_hexMoreThan4 = "0x7496efba1231507976";

    uint32_t exp_hexMoreThan4[] = {
        0x31507976, 0x96efba12, 0x74, 0, 0, 0, 0, 0};

    char *key_leadingZeroes = "0x0000014a57f38de13470900";

    uint32_t exp_leadingZeroes[] = {
        0x13470900, 0xa57f38de, 0x14, 0, 0, 0, 0, 0};

    char *key_huge = "0x6C6F6E6761727320322063686C7573203330207020697320206B657954686973";

    uint32_t exp_huge[] = {
        0x54686973, 0x206B6579, 0x20697320, 0x33302070, 0x6C757320, 0x32206368, 0x61727320, 0x6C6F6E67};

    char *key_str1 = "HelloWorld";
    char *key_hex1 = "0x48656C6C6F576F726C64";

    char *key_str2 = "This is a bit long maybe no? Well now yes";
    char *key_hex2 = "0x20626974206C6F6E67206D61796265206E6F3F2057656C6C206E6F7720796573";

    printf("\n\n-- Tests for key input: --\n\n");
    if (!run_test(key_shorterThan32, exp_shorterThan32, actual))
    {
        exit(EXIT_FAILURE);
    }

    if (!run_test(key_exactly32, exp_exactly32, actual))
    {
        exit(EXIT_FAILURE);
    }

    if (!run_test(key_moreThan32, exp_moreThan32, actual))
    {
        exit(EXIT_FAILURE);
    }

    if (!run_test(key_hexShorterThan4, exp_hexShorterThan4, actual))
    {
        exit(EXIT_FAILURE);
    }

    if (!run_test(key_hexExactly4, exp_hexExactly4, actual))
    {
        exit(EXIT_FAILURE);
    }

    if (!run_test(key_hexMoreThan4, exp_hexMoreThan4, actual))
    {
        exit(EXIT_FAILURE);
    }

    if (!run_test(key_leadingZeroes, exp_leadingZeroes, actual))
    {
        exit(EXIT_FAILURE);
    }
    if (!run_test(key_huge, exp_huge, actual))
    {
        exit(EXIT_FAILURE);
    }
    if (!compareHexWithString(key_hex1, key_str1))
    {
        exit(EXIT_FAILURE);
    }
    if (!compareHexWithString(key_hex2, key_str2))
    {
        exit(EXIT_FAILURE);
    }
    printf("\n\n-- Tests for key input successful --\n\n");
}