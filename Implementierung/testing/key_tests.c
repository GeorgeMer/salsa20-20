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

void test_keys()
{

    uint32_t actual[8];

    char *key_shorterThan32 = "A short key";

    uint32_t exp_shorterThan32[] = {
        0, 0, 0, 0, 0, 0x6B657900, 0x6F727420, 0x41207368};

    char *key_exactly32 = "This key is 30 plus 2 chars long";

    uint32_t exp_exactly32[] = {
        0x6C6F6E67, 0x61727320, 0x32206368, 0x6C757320, 0x33302070, 0x20697320, 0x206B6579, 0x54686973};

    char *key_moreThan32 = "This key is a lot longer than just the measly 32 bytes";

    uint32_t exp_moreThan32[] = {
        0x6E206A75, 0x20746861, 0x6E676572, 0x74206C6F, 0x61206C6F, 0x20697320, 0x206B6579, 0x54686973};

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
    printf("\n\n-- Tests for key input successfull --\n\n");
}