#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "salsa20.h"

const char *usage_msg =
    "Usage: %s [-V X | -B X | -k K | -i I | -o P | -h | --help] file_path    Encrypt a file with Salsa20 encryption\n"
    "   or: %s -h                                                            Show help message and exit\n"
    "   or: %s --help                                                        Show help message and exit\n";
;
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
int main(int argc, char **argv)
{
    const char *progname = argv[0];
    int opt;

    if (argc < 4)
    {
        print_usage(progname);
        return EXIT_FAILURE;
    }

    // option parsing
    while (opt = getopt(argc, argv, "V:B:k:i:o:h") != -1)
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

    if (optind >= argc)
    {
        printf("%s: Missing positional argument -- 'file'\n", progname);
        print_usage(progname);
        return EXIT_FAILURE;
    }

    const char *input_path = argv[optind];

    // read the file and start the encryption
}