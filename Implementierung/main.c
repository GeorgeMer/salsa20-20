#define _POSIX_C_SOURCE 199309L
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <inttypes.h>

#include "file_IO/file_IO.h"
#include "number_conversions/number_conversions.h"

#include "salsa_crypt/salsa_crypt_v0.h"
#include "salsa_crypt/salsa_crypt_v1.h"
#include "salsa_crypt/salsa_crypt_v2.h"

#include "testing/reference/correctness.h"
#include "testing/key_tests.h"
#include "testing/crypt_tests.h"
#include "testing/input_tests.h"

static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
};

size_t mlen = 0;

const char *usage_msg =
    "Usage: %s [-V X | -B X | -t X | -k K | -i I | -o P | -h | --help] file_path    Encrypt/Decrypt a file with Salsa20 encryption\n"
    "   or: %s -h                                                                   Show help message and exit\n"
    "   or: %s --help                                                               Show help message and exit\n";

const char *help_msg =
    "Positional arguments:\n"
    "  path   The path to the file to be encrypted.\n"
    "\n"
    "Optional arguments:\n"
    "  -V X   The implementation to be used (default: X = 0)\n"
    "         Can be given as a hex with the prefix '0x', Octal with prefix '0' or as a decimal number with no prefix.\n"
    "  -B X   If set, runtime of chosen implementation will be measured and output.\n"
    "         X represents the number of repetition of function calls (default: No runtime measurement)\n"
    "         Can be given as a hex with the prefix '0x', Octal with prefix '0' or as a decimal number with no prefix.\n"
    "  -t X   If set, tests will be executed that compare the chosen implementation with the reference implementation.\n"
    "         X represents the number of random tests in addition to the set tests that will be executed. (default: No tests are run)\n"
    "         (all tests run with random keys and nonces, random tests also have random messages)\n"
    "  -k K   K resembles the 256bit long key (default: K = 2^237 - 1711)\n"
    "         The key can be either given as a hex with the '0x' prefix (it mustn't be greater than 2^256-1, but can have leading zeroes which are ignored),\n"
    "         or as a sequence of characters(bytes). If the given string is longer than 32 characters, only the last 32 characters will make up the key.\n"
    "         If the string contains a space character, the string has to be surrounded by (double or single) quotation marks.\n"
    "         Therefore, quotation marks are not taken into consideration when converting the string into the key.\n"
    "  -i I   I resembles the initialization vector (default: I = 2^59 - 427)\n"
    "         Can be given as a hex with the prefix '0x', Octal with prefix '0' or as a decimal number with no prefix.\n"
    "  -o P   P is the path to the output file\n"
    "  -h     Show help message (this text) and exit\n"
    "  --help Same effect as -h\n"
    "\n"
    "Please note:\n"
    "  Negative Numbers are not allowed.\n"
    "  If -o is not set, the program will create an output file with the name result.txt in the current directory.\n"
    "  To decrypt a file, just call the program with the output file (of the previous encryption) as input file and with the same key and nonce already used for encryption.\n"
    "  If the file contains any key whose respective ASCII value is not in [32;126], decryption will not yield the expected result.\n";

void print_usage(const char *progname) { fprintf(stderr, usage_msg, progname, progname, progname); }
void print_help(const char *progname)
{
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}

double gettime_in_seconds(const struct timespec start, const struct timespec end)
{
    return (double)((((end.tv_sec - start.tv_sec) * 1000000000)) + (((end.tv_nsec - start.tv_nsec)))) / 1000000000;
}

int main(int argc, char **argv)
{
    const char *progname = argv[0];
    int opt;

    if (argc == 1)
    {
        print_usage(progname);
        return EXIT_FAILURE;
    }

    // default values
    uint64_t implementation_number = 0;
    uint64_t number_of_iterations = 0;
    uint64_t random_tests = 0;
    bool run_tests = false;
    bool measure_runtime = false;
    uint32_t key[8];
    key[0] = 0xfffff951;
    key[1] = 0xffffffff;
    key[2] = 0xffffffff;
    key[3] = 0xffffffff;
    key[4] = 0xffffffff;
    key[5] = 0xffffffff;
    key[6] = 0xffffffff;
    key[7] = 0x1fff;

    uint64_t iv = 0x7ffffff76b48c40;
    const char *output_path = NULL;

    // option parsing
    while ((opt = getopt_long(argc, argv, "V:B:t:k:i:o:h", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'V':
            implementation_number = convert_string_to_uint64_t(optarg);

            if (implementation_number > 2)
            {
                fprintf(stderr, "This implementation does not exist! Valid implementation numbers are 0, 1 and 2.\n");
                return EXIT_FAILURE;
            }

            break;
        case 'B':
            number_of_iterations = convert_string_to_uint64_t(optarg);
            measure_runtime = true;

            if (number_of_iterations == 0)
            {
                fprintf(stderr, "You can't measure the runtime of 0 iterations.\n");
                return EXIT_FAILURE;
            }

            break;
        case 't':
            random_tests = convert_string_to_uint64_t(optarg);
            run_tests = true;
            break;
        case 'k':;
            convert_string_to_uint32_t_array(optarg, key);
            break;
        case 'i':
            iv = convert_string_to_uint64_t(optarg);
            break;
        case 'o':
            output_path = optarg;
            break;
        case 'h':
            print_help(progname);
            return EXIT_SUCCESS;
        default:
            print_usage(progname);
            return EXIT_FAILURE;
        }
    }

    // checking if path to input file is missing
    if (optind >= argc)
    {
        printf("%s: Missing positional argument -- 'file'\n", progname);
        print_usage(progname);
        return EXIT_FAILURE;
    }
    else if (optind < argc - 1)
    {
        printf("%s: You can only enter one input file\n", progname);
        print_usage(progname);
        return EXIT_FAILURE;
    }

    const char *input_path = argv[optind];

    /*if run_tests is set to true, run the tests comparing implementation (given by 1st argument) with reference,
    run as many random tests as specified by the 2nd argument in addition to that.

    -- All tests run with randomly generated keys and nonces --
    */
    if (run_tests)
    {
        printf("\n\n------ TESTS: ------\n\n\n");
        test_correctness(implementation_number, random_tests);
        test_crypt(implementation_number, random_tests);
        test_keys();
        input_tests(implementation_number, random_tests, input_path, output_path);
        printf("\n\n------ END OF TESTS ------\n\n\n");
    }

    // read the file and start the encryption using implementation according to implementation_number

    const uint8_t *message = read_file(input_path);
    uint8_t *cipher = malloc(mlen);

    if (cipher == NULL)
    {
        fprintf(stderr, "Couldn't allocate enough memory.\n");
        return EXIT_FAILURE;
    }

    // if measure_runtime == true then measure runtime with function being called number_of_iterations times
    if (measure_runtime)
    {
        struct timespec start;
        struct timespec end;

        // error handling
        if (clock_gettime(CLOCK_MONOTONIC, &start))
        {
            fprintf(stderr, "Something went wrong measuring the runtime.\n");
            return EXIT_FAILURE;
        }

        // calling the function number_of_iterations times
        for (uint64_t i = 1; i <= number_of_iterations; i++)
        {
            switch (implementation_number)
            {
            case 0:
                salsa20_crypt(mlen, message, cipher, key, iv);
                break;
            case 1:
                salsa20_crypt_v1(mlen, message, cipher, key, iv);
                break;
            case 2:
                salsa20_crypt_v2(mlen, message, cipher, key, iv);
                break;
            default:
                break;
            }
        }

        // error handling
        if (clock_gettime(CLOCK_MONOTONIC, &end))
        {
            fprintf(stderr, "Something went wrong measuring the runtime.\n");
            return EXIT_FAILURE;
        }
        double time = gettime_in_seconds(start, end);
        double per_iter = time / number_of_iterations;

        // printing the result to the console
        printf("\n\nThe runtime for implementation "
               "%" PRIu64 " with "
               "%" PRIu64 " function calls amounts to "
               "%f seconds.\nEach iteration took %f seconds.\n\n",
               implementation_number, number_of_iterations, time, per_iter);
    }

    // run one iteration of the algorithm
    switch (implementation_number)
    {
    case 0:
        salsa20_crypt(mlen, message, cipher, key, iv);
        break;
    case 1:
        salsa20_crypt_v1(mlen, message, cipher, key, iv);
        break;
    case 2:
        salsa20_crypt_v2(mlen, message, cipher, key, iv);
        break;
    default:
        break;
    }

    // free input and cipher pointer and write to output file
    write_file(output_path, cipher);
    free((void *)message);
    free((void *)cipher);
    return EXIT_SUCCESS;
}