#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "reference.h"
#include "../../salsa_crypt/salsa_crypt_v0.h"
#include "../../salsa_crypt/salsa_crypt_v1.h"
#include "../../salsa_crypt/salsa_crypt_v2.h"
#include "../../salsa_core/salsa_core_v0.h"
#include "../../salsa_core/salsa_core_v1.h"
#include "../../salsa_core/salsa_core_v2.h"
#include "../asserts.h"
#include "../print_tests.h"
#include "../random_generation/random_numbers.h"
#include "../random_generation/random_msg_key_iv.h"

// runs a test case
bool run_testcase(uint64_t implementation, size_t mlen, const uint8_t msg[mlen])
{
    // initialize values
    srand(mlen + msg[0] + time(NULL));
    uint32_t key[8];
    uint64_t iv;
    uint8_t cipher[mlen];
    uint8_t cipher_reference[mlen];

    // random key and nonce
    randomKey(key);
    iv = randomNonce();

    // print debug
    printf("Message: ");
    print_char_arr(mlen, msg);
    printf("Key: ");
    print_arr_32bit(8, key);
    printf("Nonce: %lx\n\n", iv);

    switch (implementation)
    {
    case 1:
        salsa20_crypt_v1(mlen, msg, cipher, key, iv);
        break;
    case 2:
        salsa20_crypt_v2(mlen, msg, cipher, key, iv);
        break;
    default:
        salsa20_crypt(mlen, msg, cipher, key, iv);
        break;
    }

    // reference implementation
    salsa_crypt_reference(mlen, msg, cipher_reference, key, iv);

    printf("Cipher from implementation %lu:\n", implementation);
    print_arr_8bit(mlen, cipher);
    printf("Cipher from reference:\n");
    print_arr_8bit(mlen, cipher_reference);
    printf("\n");

    // assert
    return assertEqualsArrays_8bit(mlen, cipher_reference, mlen, cipher);
}

// executes a random test
bool execute_random(uint64_t implementation)
{
    // random message
    size_t mlen = randomInt(10, 1025);
    uint8_t msg[mlen];
    randomMsg(mlen, msg);

    // runs testcase
    return run_testcase(implementation, mlen, msg);
}

// tests core function
bool test_specification(uint64_t implementation, uint32_t input[16], uint32_t expected[16])
{
    // print debug
    printf("Input:\n");
    print_arr_32bit(16, input);
    printf("\n\n");
    uint32_t actual[16];

    // run test to get actual
    switch (implementation)
    {
    case 1:
        salsa20_core_v1(actual, input);
        break;
    case 2:
        salsa20_core_v2(actual, input);
        break;
    default:
        salsa20_core(actual, input);
        break;
    }

    // print debug
    printf("After calling salsa on input: \n");
    print_arr_32bit(16, actual);
    printf("Expected: \n");
    print_arr_32bit(16, expected);

    // assert
    return assertEqualsArrays_32bit(16, expected, 16, actual);
}

void test_correctness(uint64_t implementation, uint64_t random_tests)
{
    // initialize messages to test
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

    // if run_testcase fails assertion, break program execution
    printf("\n\n\n-- Core tests: --\n\n");
    printf("\n\n- Test 1:\n\n");
    if (!run_testcase(implementation, sizeof(msg_lessthan64), msg_lessthan64))
    {
        exit(EXIT_FAILURE);
    }
    printf("\n\n- Test 2:\n\n");
    if (!run_testcase(implementation, sizeof(msg_morethan64), msg_morethan64))
    {
        exit(EXIT_FAILURE);
    }
    printf("\n\n- Test 3:\n\n");
    if (!run_testcase(implementation, sizeof(msg_exactly64), msg_exactly64))
    {
        exit(EXIT_FAILURE);
    }

    // run a number of random tests, break if assertion fails
    for (uint64_t i = 0; i < random_tests; i++)
    {
        printf("\n\n- Random Test %lu:\n\n", i + 1);
        if (!execute_random(implementation))
        {
            exit(EXIT_FAILURE);
        }
    }

    // specification tests (see section 8 of salsa specification (https://cr.yp.to/snuffle/spec.pdf) )
    uint32_t spec1[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t spec1_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint32_t spec2[16] = {0X730D9FD3, 0XB752374C, 0X25DE7503, 0X88EABBBF, 0X30B3ED31, 0XDBB26A01, 0X30A6C7AF, 0XCFB31056, 0X3F20F01F, 0XA15D530F, 0X71309374, 0X24CC37EE, 0X4FEBC94F, 0X2F9C5103, 0XF3F41ACB, 0X36687658};
    uint32_t spec2_exp[16] = {0XA8B22A6D, 0XEEF8F09C, 0XCBBEC4A8, 0X9AAA6E1A, 0X1A961D1D, 0XF9EB1E96, 0X30FBA3BE, 0X39339045, 0X9D982876, 0X5E1B39B4, 0X23EC2A6B, 0X72726F1B, 0X87E8ECDB, 0X126E9B6F, 0X9E5FE818, 0XCA3013B3};

    uint32_t spec3[16] = {0x36687658, 0X4FEBC94F, 0X2F9C5103, 0XF3F41ACB, 0X88EABBBF, 0X730D9FD3, 0XB752374C, 0X25DE7503, 0XCFB31056, 0X30B3ED31, 0XDBB26A01, 0X30A6C7AF, 0X24CC37EE, 0X3F20F01F, 0XA15D530F, 0X71309374};
    uint32_t spec3_exp[16] = {0XCA3013B3, 0X87E8ECDB, 0X126E9B6F, 0X9E5FE818, 0X9AAA6E1A, 0XA8B22A6D, 0XEEF8F09C, 0XCBBEC4A8, 0X39339045, 0X1A961D1D, 0XF9EB1E96, 0X30FBA3BE, 0X72726F1B, 0X9D982876, 0X5E1B39B4, 0X23EC2A6B};

    printf("- Specification Test 1:\n\n");
    if (!test_specification(implementation, spec1, spec1_exp))
    {
        exit(EXIT_FAILURE);
    }
    printf("- Specification Test 2:\n\n");
    if (!test_specification(implementation, spec2, spec2_exp))
    {
        exit(EXIT_FAILURE);
    }
    printf("- Specification Test 3:\n\n");
    if (!test_specification(implementation, spec3, spec3_exp))
    {
        exit(EXIT_FAILURE);
    }

    printf("\n-- Executed all core tests successfully!--\n\n");
}
