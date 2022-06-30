#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include "salsa20.h"

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
    "  -B X   If set, runtime of chosen implementation will be measured and output. X represents the number of repetition of function calls\n"
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
        perror("Something went wrong opening your file. Did you enter the correct path?");
        goto error;
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
    char *endptr;
    uint64_t result = strtoull(string, &endptr, 0);
    errno = 0;

    // error handling
    if (endptr == string || *endptr != '\0')
    {
        fprintf(stderr, "%s could not be converted to uint64_t\n", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == ERANGE)
    {
        fprintf(stderr, "%s over - or underflows uint64_t", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == EINVAL)
    {
        fprintf(stderr, "%s No conversion could be performed from String to uint64_t", string);
        exit(EXIT_FAILURE);
    }
    return result;
}

uint32_t convert_string_to_uint32_t(const char *string)
{
    char *endptr;
    uint32_t result = strtoul(string, &endptr, 0);
    errno = 0;

    // error handling
    if (endptr == string || *endptr != '\0')
    {
        fprintf(stderr, "%s could not be converted to uint32_t\n", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == ERANGE)
    {
        fprintf(stderr, "%s over - or underflows uint32_t", string);
        exit(EXIT_FAILURE);
    }
    else if (errno == EINVAL)
    {
        fprintf(stderr, "%s No conversion could be performed from String to uint32_t", string);
        exit(EXIT_FAILURE);
    }
    return result;
}

static void write_file(const char *path, const char *string)
{
    FILE *file;

    // error handling
    if (path == NULL)
    {
        // create txt file "encrypted" if no path to output file was given
        if (!(file = fopen("encrypted.txt", "w")))
        {
            perror("Something went wrong creating a file.");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (!(file = fopen(path, "w")))
        {
            perror("Something went wrong opening your output file. Did you enter the correct path?");
            exit(EXIT_FAILURE);
        }
    }

    if (!(fwrite(string, 1, strlen(string), file)))
    {
        perror("Something went wrong writing to your output file.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // close successfully written file
    fclose(file);
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

            if (implementation_number > 1)
            {
                perror("This implementation does not exist! Valid implementation numbers are 0 and 1.");
                return EXIT_FAILURE;
            }

            break;
        case 'B':
            number_of_iterations = convert_string_to_uint64_t(optarg);
            measure_runtime = true;
            break;
        case 'k':
            key[7] = convert_string_to_uint32_t(*(optarg)&255);
            key[6] = convert_string_to_uint32_t((*(optarg) >> 8) & 255);
            key[5] = convert_string_to_uint32_t((*(optarg) >> 16) & 255);
            key[4] = convert_string_to_uint32_t((*(optarg) >> 24) & 255);
            key[3] = convert_string_to_uint32_t((*(optarg) >> 32) & 255);
            key[2] = convert_string_to_uint32_t((*(optarg) >> 40) & 255);
            key[1] = convert_string_to_uint32_t((*(optarg) >> 48) & 255);
            key[0] = convert_string_to_uint32_t((*(optarg) >> 56) & 255);
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

    // read the file and start the encryption using implementation according to implementation_number
    // if measure_runtime == true then measure runtime with function being called number_of_iterations times
    // don't forget to free message again!
    const uint8_t *message = read_file(argv[optind]);
    uint8_t cipher[mlen];

    salsa20_crypt(mlen, message, cipher, key, iv);
    free(message);
    write_file(output_file, (const char *)cipher);
}