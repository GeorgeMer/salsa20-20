#include <stddef.h>
#include <stdint.h>
#include "reference.h"
#include "../../salsa_crypt/salsa_crypt_v0.h"
#include "../../salsa_crypt/salsa_crypt_v1.h"
#include "../../salsa_crypt/salsa_crypt_v2.h"
#include "../asserts.h"
#include "../print_tests.h"
#include "../random_generation/random_numbers.h"
#include "../random_generation/random_msg_key_iv.h"

void run_testcase(uint64_t implementation, size_t mlen, const uint8_t msg[mlen])
{
    uint32_t key[8];
    uint64_t iv;
    uint8_t cipher[mlen];
    uint8_t cipher_reference[mlen];

    randomKey(key);
    iv = randomNonce();

    printf("Message: ");
    print_char_arr(mlen, msg);
    printf("Key: ");
    print_arr_32bit(8, key);
    printf("Nonce: %lx", iv);

    switch (implementation)
    {
    case 1:
        salsa_crypt_v1(mlen, msg, cipher, key, iv);
        break;
    case 2:
        salsa_crypt_v2(mlen, msg, cipher, key, iv);
        break;
    default:
        salsa_crypt(mlen, msg, cipher, key, iv);
        break;
    }

    salsa_crypt_reference(mlen, msg, cipher_reference, key, iv);

    printf("Cipher from implementation %lu:\n", implementation);
    print_arr_8bit(mlen, cipher);
    printf("Cipher from reference:\n");
    print_arr_8bit(mlen, cipher_reference);
    assertEqualsArrays_8bit(mlen, cipher_reference, mlen, cipher);
}

void execute_random(uint64_t implementation)
{
    size_t mlen = randomInt(10, 80);
    uint8_t msg[mlen];
    randomMsg(mlen, msg);

    run_testcase(implementation, mlen, msg);
}

void test_correctness(uint64_t implementation, uint64_t random_tests)
{
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
        69, 105, 110, 101, 32, 78, 97, 99, 104, 114, 105, 99, 104, 116, 44, 32, 100, 105,
        101, 32, 105, 115, 116, 32, 115, 111, 32, 97, 110, 103, 101, 112, 97, 115, 115, 116,
        44, 32, 100, 97, 115, 32, 115, 105, 101, 32, 103, 101, 110, 97, 117, 32, 54, 52, 32,
        98, 105, 116, 115, 32, 105, 115, 116, 33};

    run_testcase(implementation, sizeof(msg_lessthan64), msg_lessthan64);
    run_testcase(implementation, sizeof(msg_morethan64), msg_morethan64);
    run_testcase(implementation, sizeof(msg_exactly64), msg_exactly64);

    for (uint64_t i = 0; i < random_tests; i++)
    {
        execute_random(implementation);
    }
}
