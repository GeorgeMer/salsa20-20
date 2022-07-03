#define _POSIX_C_SOURCE 199309L
#include <getopt.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

#include "salsa_crypt/salsa_crypt_v0.h"
#include "salsa_crypt/salsa_crypt_v1.h"
#include "salsa_crypt/salsa_crypt_v2.h"

#include "file_IO.h"

static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
};

size_t mlen = 0;

const char *usage_msg =
    "Usage: %s [-V X | -B X | -k K | -i I | -o P | -h | --help] file_path    Encrypt a file with Salsa20 encryption\n"
    "   or: %s -h                                                            Show help message and exit\n"
    "   or: %s --help                                                        Show help message and exit\n";

const char *help_msg =
    "Positional arguments:\n"
    "  path   The path to the file to be encrypted\n"
    "\n"
    "Optional arguments:\n"
    "  -V X   The implementation to be used (default: X = 0)\n"
    "  -B X   If set, runtime of chosen implementation will be measured and output.\n"
    "         X represents the number of repetition of function calls (default: No runtime measurement)\n"
    "  -k K   K resembles the key (default: K = 2^256 - 1883)\n"
    "  -i I   I resembles the initialization vector (default: I = 2^59 - 427)\n"
    "  -o P   P is the path to the output file\n"
    "  -h     Show help message (this text) and exit\n"
    "  --help Same effect as -h\n"
    "  All numbers can be input as decimal (no prefix) or hexadecimal (0x prefix)\n"
    "  Negative Numbers are not allowed.";

void print_usage(const char *progname) { fprintf(stderr, usage_msg, progname, progname, progname); }
void print_help(const char *progname)
{
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}

uint64_t convert_string_to_uint64_t(const char *string)
{
    if (*string == '-')
    {
        fprintf(stderr, "Negative numbers are not allowed as options.\n");
        exit(EXIT_FAILURE);
    }

    char *endptr;
    errno = 0;
    uint64_t result = strtoull(string, &endptr, 0);

    // error handling
    if (endptr == string || *endptr != '\0')
    {
        fprintf(stderr, "%s could not be converted to uint64_t\n", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == ERANGE)
    {
        fprintf(stderr, "%s over - or underflows uint64_t\n", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == EINVAL)
    {
        fprintf(stderr, "%s No conversion could be performed from String to uint64_t\n", string);
        exit(EXIT_FAILURE);
    }
    return result;
}

uint32_t convert_string_to_uint32_t(const char *string, int base)
{
    char *endptr;
    errno = 0;
    uint32_t result = strtoul(string, &endptr, base);

    // error handling
    if (endptr == string || *endptr != '\0')
    {
        fprintf(stderr, "%s could not be converted to uint32_t\n", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == ERANGE)
    {
        fprintf(stderr, "%s over - or underflows uint32_t\n", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == EINVAL)
    {
        fprintf(stderr, "%s No conversion could be performed from String to uint32_t\n", string);
        exit(EXIT_FAILURE);
    }
    return result;
}

uint64_t gettime_in_seconds(const struct timespec start, const struct timespec end)
{
    return ((uint64_t)(end.tv_sec - start.tv_sec)) + ((uint64_t)((end.tv_nsec - start.tv_nsec) / 1000000000));
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
    uint64_t number_of_iterations;
    bool measure_runtime = false;
    uint32_t key[8];
    key[0] = 0x10000001;
    key[1] = 0xc4a1da00;
    key[2] = 0x0;
    key[3] = 0x0;
    key[4] = 0x0;
    key[5] = 0x0;
    key[6] = 0x0;
    key[7] = 0x0;
    uint64_t iv = 0x7ffffff76b48c40;
    const char *output_file = NULL;

    // option parsing
    while ((opt = getopt_long(argc, argv, "V:B:k:i:o:h", long_options, NULL)) != -1)
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
            break;
        case 'k':;
            if (*(optarg) == '-')
            {
                fprintf(stderr, "Negative numbers are not allowed as options.\n");
                exit(EXIT_FAILURE);
            }

            // converting 256 bit string input number into 8 element uint_32t array
            uint8_t index = 0;
            uint8_t count = 0;
            char current_string[9];
            int base = 10;

            if (*(optarg) == '0' && *(optarg + 1) == 'x')
            {
                base = 16;
                uint8_t start = 2;
                if (strlen(optarg) > 66)
                {
                    // check for leading zeroes
                    while (*(optarg + start) == '0')
                    {
                        start++;
                    }

                    if (strlen(optarg + start) > 64)
                    {
                        fprintf(stderr, "The key entered does not fit in 256 bit.\n");
                        return EXIT_FAILURE;
                    }
                }
                optarg += start;
                size_t i = strlen(optarg) - 1, k = 0;
                for (; i >= 8; i -= 8, k++)
                {
                    char hexnum[8] = {*(optarg + i - 7), *(optarg + i - 6), *(optarg + i - 5), *(optarg + i - 4),
                                      *(optarg + i - 3), *(optarg + i - 2), *(optarg + i - 1), *(optarg + i)};
                    key[k] = convert_string_to_uint32_t(hexnum, base);
                }
                if (i != 0)
                {
                    char *lastnum = malloc(i + 2);
                    if (lastnum == NULL)
                    {
                        fprintf(stderr, "Couldn't allocate memory\n");
                        return EXIT_FAILURE;
                    }
                    for (size_t j = 0; j <= i; j++)
                    {
                        *(lastnum + j) = *(optarg + j);
                    }
                    *(lastnum + i + 1) = '\0';
                    key[k] = convert_string_to_uint32_t(lastnum, base);

                    free(lastnum);
                }
                else
                {

                    char *lastnum = malloc(2);
                    if (lastnum == NULL)
                    {
                        fprintf(stderr, "Couldn't allocate memory\n");
                        return EXIT_FAILURE;
                    }
                    *lastnum = *optarg;
                    *(lastnum + 1) = '\0';
                    key[k] = convert_string_to_uint32_t(lastnum, base);
                    free(lastnum);
                }
                for (int i = 0; i < 8; i++)
                {
                    printf("%08x ", key[i]);
                }
            }

            else
            {

                if (strlen(optarg) > 77)
                {
                    fprintf(stderr, "The key entered does not fit in 256 bit.\n");
                    return EXIT_FAILURE;
                }
            }

            while (*(optarg) != '\0')
            {
                current_string[count++] = *(optarg);
                optarg += 1;
                if (count == 8)
                {
                    current_string[count] = '\0';
                    count = 0;
                    key[index++] = convert_string_to_uint32_t(current_string, base);
                }
            }

            if (count != 0)
            {
                while (count < 9)
                {
                    current_string[count++] = '0';
                }
                current_string[count] = '\0';
                key[index++] = convert_string_to_uint32_t(current_string, base);
            }

            for (int i = index; i < 8; i++)
            {
                key[i] = 0x0;
            }
            break;
        case 'i':
            iv = convert_string_to_uint64_t(optarg);
            break;
        case 'o':
            output_file = optarg;
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

    // read the file and start the encryption using implementation according to implementation_number

    const uint8_t *message = read_file(argv[optind]);
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
                salsa_crypt(mlen, message, cipher, key, iv);
                break;
            case 1:
                salsa_crypt_v1(mlen, message, cipher, key, iv);
                break;
            case 2:
                salsa_crypt_v2(mlen, message, cipher, key, iv);
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

        // printing the result to the console
        printf("The runtime for implementation "
               "%" PRIu64 " with "
               "%" PRIu64 " function calls amounts to "
               "%" PRIu64 " seconds.\n",
               implementation_number, number_of_iterations, gettime_in_seconds(start, end));
    }
    else
    {
        switch (implementation_number)
        {
        case 0:
            salsa_crypt(mlen, message, cipher, key, iv);
            break;
        case 1:
            salsa_crypt_v1(mlen, message, cipher, key, iv);
            break;
        case 2:
            salsa_crypt_v2(mlen, message, cipher, key, iv);
            break;
        default:
            break;
        }
    }

    // free input pointer and write to output file
    free((void *)message);
    write_file(output_file, cipher);
    return EXIT_SUCCESS;
}