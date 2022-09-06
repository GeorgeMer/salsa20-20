#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "../salsa_crypt/salsa_crypt_v0.h"
#include "../salsa_crypt/salsa_crypt_v1.h"
#include "../salsa_crypt/salsa_crypt_v2.h"
#include "../testing/print_tests.h"
#include "../testing/asserts.h"
#include "../testing/random_generation/random_numbers.h"
#include "../testing/random_generation/random_msg_key_iv.h"

// run a testcase
bool run_testcase_crypt(uint64_t implementation, size_t mlen, const uint8_t msg[mlen])
{
    // initialize and get random key and nonce
    uint8_t cipher[mlen], final[mlen];

    srand(mlen + msg[0] + time(NULL));
    uint32_t key[8];
    uint64_t iv;
    randomKey(key);
    iv = randomNonce();

    // run twice to then compare final with original
    switch (implementation)
    {
    case 1:
        salsa20_crypt_v1(mlen, msg, cipher, key, iv);
        salsa20_crypt_v1(mlen, cipher, final, key, iv);
        break;
    case 2:
        salsa20_crypt_v2(mlen, msg, cipher, key, iv);
        salsa20_crypt_v2(mlen, cipher, final, key, iv);
        break;
    default:
        salsa20_crypt(mlen, msg, cipher, key, iv);
        salsa20_crypt(mlen, cipher, final, key, iv);
        break;
    }

    // print debug
    printf("Original message: ");
    print_char_arr(mlen, msg);
    printf("\nFinal message after two crypt calls: ");
    print_char_arr(mlen, final);
    printf("\n");

    // assert original equals with output of 2 crypt calls
    return assertEqualsArrays_8bit(mlen, msg, mlen, final);
}

// sets up test on random message
bool execute_random_crypt(uint64_t implementation)
{
    size_t mlen = randomInt(10, 1025);
    uint8_t msg[mlen];
    randomMsg(mlen, msg);

    return run_testcase_crypt(implementation, mlen, msg);
}

void test_crypt(uint64_t implementation, uint64_t random_tests)
{
    // initialize test values
    uint8_t msg_lessthan64[] = {
        68, 105, 101, 115, 101, 32, 78, 97, 99, 104, 114, 105, 99, 104, 116,
        32, 105, 115, 116, 32, 107, 117, 101, 114, 122, 101, 114, 32, 97, 108, 115, 32, 54, 52, 32, 98, 121, 116, 101, 115};

    uint8_t msg_morethan64[] = {

        68, 105, 101, 115, 101, 32, 102, 111, 108, 103, 101, 110, 100, 101, 32, 78, 97, 99, 104, 114, 105, 99, 104, 116, 32, 115, 111, 108, 108, 32, 108, 97, 101, 110, 103, 101, 114, 32, 97, 108,
        115, 32, 54, 52, 32, 98, 121, 116, 101, 115, 32, 115, 101, 105, 110, 46, 32, 68, 97, 109, 105, 116, 32, 105, 99, 104, 32, 100, 97, 115, 115, 32, 98, 101, 107, 111, 109, 109, 101, 44, 32, 109, 117, 115,
        115, 32, 105, 99, 104, 32, 101, 105, 110, 32, 103, 97, 110, 122, 32, 103, 114, 111, 115, 115, 101, 115, 32, 84, 101, 120, 116, 32, 97, 117, 102, 32, 68, 101, 117, 116, 115, 99, 104, 32, 115, 99, 104,
        114, 101, 105, 98, 101, 110, 44, 32, 111, 98, 119, 111, 104, 108, 32, 68, 101, 117, 116, 115, 99, 104, 32, 110, 105, 99, 104, 116, 32, 109, 101, 105, 110, 101, 32, 77, 117, 116, 116, 101, 114, 115,
        112, 114, 97, 99, 104, 101, 32, 105, 115, 116, 46, 32, 85, 101, 98, 114, 105, 103, 101, 110, 115, 44, 32, 102, 105, 110, 100, 101, 32, 105, 99, 104, 32, 100, 97, 115, 32, 103, 97, 110, 122, 101, 32,
        80, 114, 111, 106, 101, 107, 116, 32, 115, 101, 104, 114, 32, 116, 111, 108, 108, 46, 32, 75, 114, 121, 112, 116, 111, 103, 114, 97, 112, 104, 105, 101, 32, 105, 115, 116, 32, 101, 99, 104, 116, 32,
        103, 101, 105, 108, 44, 32, 97, 98, 101, 114, 32, 118, 101, 114, 109, 117, 116, 108, 105, 99, 104, 32, 110, 97, 101, 99, 104, 115, 116, 101, 115, 32, 77, 97, 108, 32, 110, 105, 99, 104, 116, 32,
        105, 110, 32, 67, 32, 117, 110, 100, 32, 110, 105, 99, 104, 116, 32, 97, 117, 102, 32, 68, 101, 117, 116, 115, 99, 104, 46, 32

    };

    uint8_t msg_exactly64[] = {
        69, 105, 110, 101, 32, 78, 97, 99, 104, 114, 105, 99, 104, 116, 44, 32, 100, 105, 101, 32, 105, 115, 116, 32,
        115, 111, 32, 97, 110, 103, 101, 112, 97, 115, 115, 116, 44, 32, 100, 97, 115, 32, 115, 105, 101, 32, 103, 101, 110, 97, 117,
        32, 54, 52, 32, 98, 121, 116, 101, 115, 32, 105, 115, 116};

    // if run_testcase_crypt returns false, break execution
    printf("\n\n-- Crypt tests: --\n\n");
    printf("- Test 1:\n\n");
    if (!run_testcase_crypt(implementation, sizeof(msg_lessthan64), msg_lessthan64))
    {
        exit(EXIT_FAILURE);
    }
    printf("- Test 2:\n\n");
    if (!run_testcase_crypt(implementation, sizeof(msg_morethan64), msg_morethan64))
    {
        exit(EXIT_FAILURE);
    }
    printf("- Test 3:\n\n");
    if (!run_testcase_crypt(implementation, sizeof(msg_exactly64), msg_exactly64))
    {
        exit(EXIT_FAILURE);
    }

    // execute a number of random tests
    for (uint64_t i = 0; i < random_tests; i++)
    {
        printf("- Random Test %lu:\n\n", i + 1);
        if (!execute_random_crypt(implementation))
        {
            exit(EXIT_FAILURE);
        }
    }

    printf("-- Executed all crypt tests successfully!--\n\n\n");
}