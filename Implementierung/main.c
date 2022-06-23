#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/stat.h>
#include "salsa20.h"

static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
};

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
    "  -B X   If set, runtime of chosen implementation will be mesured and output. X represents the number of repition of function calls\n"
    "  -k K   K resemlbes the key\n"
    "  -i I   I resemlbes the initialization vector\n"
    "  -o P   P is the path to the output file\n"
    "  -h     Show help message (this text) and exit\n"
    "  --help Same effect as -h\n";

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
        fprintf("Error reading file: Could not allocate enough memory\n");
        goto error;
    }

    if (!fread(message, 1, statbuf.st_size, file))
    {
        perror("Error reading file");
        free(message);
        goto error;
    }

    fclose(file);
    message[statbuf.st_size] = '\0';
    return (const uint8_t *)message;

error:
    fclose(file);
    return EXIT_FAILURE;
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

    // option parsing
    while ((opt = getopt_long(argc, argv, "V:B:k:i:o:h", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'V':
            break;
        case 'B':
            break;
        case 'k':
            break;
        case 'i':
            break;
        case 'o':
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

    // read the file and start the encryption
    const uint8_t *message = read_file(argv[optind]);
}