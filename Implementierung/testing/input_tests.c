#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "../file_IO/file_IO.h"
#include "../salsa_crypt/salsa_crypt_v0.h"
#include "../salsa_crypt/salsa_crypt_v1.h"
#include "../salsa_crypt/salsa_crypt_v2.h"
#include "../testing/print_tests.h"
#include "../testing/asserts.h"
#include "../testing/random_generation/random_numbers.h"
#include "../testing/random_generation/random_msg_key_iv.h"

bool input_test(uint64_t implementation_number, const char *input_path)
{
    // get input file permissions
    struct stat fs;
    int r, input_perms = 384; // === 0600

    r = stat(input_path, &fs);
    if (r == -1)
    {
        fprintf(stderr, "Error, couldn't read input file permissions\n");
    }
    else
    {
        input_perms = fs.st_mode;
    }

    uint8_t *msg;
    uint32_t key[8];
    uint64_t iv;

    // set output path used for testing
    const char *output_path = "testing/io_binfiles/output.bin";

    // get input
    msg = read_file(input_path);

    // random nonce and key
    srand(mlen + msg[mlen - 1] + time(NULL));
    randomKey(key);
    iv = randomNonce();

    uint8_t cipher[mlen];

    // run crypt
    switch (implementation_number)
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
    free(msg); // variable will be used again

    // write to output
    write_file(output_path, cipher, input_perms);

    // read output as input for second crypt function run
    msg = read_file(output_path);

    // run crypt
    switch (implementation_number)
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
    // write to output
    write_file(output_path, cipher, input_perms);

    // expected and actual byte arrays representing binary of files
    uint8_t *expected, *actual;

    // read and print input
    expected = read_file(input_path);
    size_t expLen = mlen;
    printf("Input: ");
    print_arr_8bit(expLen, expected);
    printf("\n");

    // read and print output
    actual = read_file(output_path);
    size_t actLen = mlen;
    printf("Output after double encryption: ");
    print_arr_8bit(actLen, actual);
    printf("\n");

    // get input and output permissions
    int exp_perms, act_perms;

    // input perms
    r = stat(input_path, &fs);
    if (r == -1)
    {
        fprintf(stderr, "Error, couldn't read file permissions\n");
        return false;
    }
    else
    {
        exp_perms = fs.st_mode;
    }

    // output perms
    r = stat(output_path, &fs);
    if (r == -1)
    {
        fprintf(stderr, "Error, couldn't read file permissions\n");
        return false;
    }
    else
    {
        act_perms = fs.st_mode;
    }

    // compare permissions
    if (exp_perms != act_perms)
    {
        printf("Permissions of input and output file don't match: %d != %d\n", exp_perms, act_perms);
        return false;
    }

    // compare byte arrays
    bool ret = (assertEqualsArrays_8bit(expLen, expected, actLen, actual));

    // deallocate and return
    free(expected);
    free(actual);
    free(msg);
    return ret;
}

bool random_input_test(uint64_t implementation_number)
{
    // a set input path
    const char *input_path = "testing/io_binfiles/input.bin";

    // default permissions
    int input_perms = 384; // === 0600

    // generate random values
    mlen = randomInt(10, 1025);
    uint8_t msg[mlen];
    randomMsg(mlen, msg);

    // write random message to to input
    write_file(input_path, msg, input_perms);

    // execute test
    return input_test(implementation_number, input_path);
}

void input_tests(uint64_t implementation_number, uint64_t random_tests, const char *input_path)
{
    printf("\n\n-- Input tests: ---\n\n\n - Manual Test 1:\n\n");

    // run manual test
    if (!input_test(implementation_number, input_path))
    {
        exit(EXIT_FAILURE);
    }

    // run random_tests number of random tests
    for (uint64_t i = 0; i < random_tests; i++)
    {
        printf("- Random Test %lu:\n\n", i);
        if (!random_input_test(implementation_number))
        {
            exit(EXIT_FAILURE);
        }
    }

    printf("-- All input tests succesfull -- \n\n");
}
