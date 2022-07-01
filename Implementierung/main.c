#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

#include "salsa_crypt/salsa_crypt_v0.h"
#include "salsa_crypt/salsa_crypt_v1.h"
#include "salsa_crypt/salsa_crypt_v2.h"

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
    "  -B X   If set, runtime of chosen implementation will be measured and output. X represents the number of repetition of function calls (default: No runtime measurement)\n"
    "  -k K   K resembles the key\n (default: K = 2^256 - 1883)"
    "  -i I   I resembles the initialization vector (default: I = 2^59 - 427)\n"
    "  -o P   P is the path to the output file\n"
    "  -h     Show help message (this text) and exit\n"
    "  --help Same effect as -h\n"
    "  All numbers can be input as decimal (no prefix), octal (0 prefix) and hexadecimal (0x prefix)\n";

void print_usage(const char *progname) { fprintf(stderr, usage_msg, progname, progname, progname); }
void print_help(const char *progname)
{
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}

const uint8_t *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    char *message = NULL;

    // error handling
    if (file == NULL)
    {
        perror("Something went wrong opening your file");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    if (fstat(fileno(file), &statbuf))
    {
        perror("Error retrieving file stats");
        goto error;
    }

    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0)
    {
        fprintf(stderr, "Error processing file: Not a regular file or invalid size\n");
        goto error;
    }

    if (!(message = malloc(statbuf.st_size + 1)))
    {
        fprintf(stderr, "Error reading file: Could not allocate enough memory\n");
        goto error;
    }

    if (!fread(message, 1, statbuf.st_size, file))
    {
        perror("Error reading file");
        free(message);
        goto error;
    }

    // close successfully read file, add 0 byte add the end of char array and convert it to uint8_t array
    fclose(file);
    message[statbuf.st_size] = '\0';
    mlen = statbuf.st_size;
    return (const uint8_t *)message;

    // label to jump to if error occured reading input file
    // closes file and ends the program by returning EXIT_FAILURE
error:
    fclose(file);
    exit(EXIT_FAILURE);
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

static void write_file(const char *path, uint8_t *cipher)
{
    FILE *file;

    // error handling
    if (path == NULL)
    {
        // create txt file "encrypted" if no path to output file was given
        if (!(file = fopen("encrypted.txt", "w")))
        {
            perror("Something went wrong creating a file");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (!(file = fopen(path, "w")))
        {
            perror("Something went wrong opening your output file");
            exit(EXIT_FAILURE);
        }
    }

    // writing cipher to output file as string of hexadecimals

    for (size_t i = 0; i < mlen; i++)
    {
        if (*(cipher + i) == 0)
        {
            // writing 0 to output file as 00
            if (fprintf(file, "00") < 0)
            {
                perror("Something went wrong writing to your output file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        else if (*(cipher + i) <= 15)
        {
            // writing single digit hex to output file as 0z
            if (fprintf(file, "0%x", *(cipher + i)) < 0)
            {
                perror("Something went wrong writing to your output file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // writing double digit hex to output file
            if (fprintf(file, "%x", *(cipher + i)) < 0)
            {
                perror("Something went wrong writing to your output file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }
    // close successfully written file
    fclose(file);
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
            // converting 256 bit string input number into 8 element uint_32t array
            uint8_t index = 0;
            uint8_t count = 0;
            char current_string[9];
            int base = 10;

            // TODO: catch too large input
            if (*(optarg) == '0' && *(optarg + 1) == 'x')
            {
                if (strlen(optarg) > 67)
                {
                    fprintf(stderr, "The key entered does not fit in 256 bit.\n");
                    return EXIT_FAILURE;
                }
                base = 16;
                optarg += 2;
            }
            else if (*(optarg) == '0')
            {
                base = 8;
                optarg += 1;
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
                salsa_crypt_v0(mlen, message, cipher, key, iv);
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
        return EXIT_SUCCESS;
    }
    else
    {
        switch (implementation_number)
        {
        case 0:
            salsa_crypt_v0(mlen, message, cipher, key, iv);
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