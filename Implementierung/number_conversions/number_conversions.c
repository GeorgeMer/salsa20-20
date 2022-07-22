#include "number_conversions.h"

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

void convert_string_to_uint32_t_array(const char *string, uint32_t key[8])
{
    key[0] = 0;
    key[1] = 0;
    // given string is interpreted as a hex number
    for (size_t i = 0; i < 8; i++)
    {
        key[i] = 0;
    }

    // given string is interpreted as a hex number
    if (*(string) == '0' && *(string + 1) == 'x')
    {

        int base = 16;
        uint8_t start = 2;
        if (strlen(string) > 66)
        {
            // check for leading zeroes
            while (*(string + start) == '0')
            {
                start++;
            }

            if (strlen(string + start) > 64)
            {
                fprintf(stderr, "The key entered does not fit in 256 bits.\n");
                exit(EXIT_FAILURE);
            }
        }
        string += start;

        size_t i = strlen(string), j = 0, k = 0;
        char hexnum[9];
        char inv[9];
        for (; i > 0; i--, j++)
        {
            // every 8 characters of the string are converted to one key
            if (j == 8)
            {
                // invert for correct endianness
                for (size_t ii = 0, jj = 7; ii < 8; ii++, jj--)
                {
                    inv[ii] = hexnum[jj];
                }
                inv[8] = '\0';

                // convert
                key[k] = convert_string_to_uint32_t(inv, base);
                k++;
                j = 0;
            }

            hexnum[j] = *(string + i - 1);
        }

        // leftover
        for (; j <= 8; j++)
        {
            if (j == 8)
            {
                for (size_t ii = 0, jj = 7; ii < 8; ii++, jj--)
                {
                    inv[ii] = hexnum[jj];
                }
                inv[8] = '\0';
                key[k] = convert_string_to_uint32_t(inv, base);
                break;
            }

            hexnum[j] = '0';
        }

        printf("Key was interpreted as hex: ");
        printf("0x");
        for (size_t i = 8; i > 0; i--)
        {
            printf("%08x", key[i - 1]);
        }
    }

    // key is interpreted as a string
    else
    {
        uint8_t chars[4];
        size_t k = 0;
        int8_t j = 3;
        for (size_t i = strlen(string); i > 0 && k < 8; i--)
        {
            // every four bytes convert to a key element
            if (j == -1)
            {
                key[k++] = ((chars[0]) << 24) | ((chars[1]) << 16) | ((chars[2]) << 8) | ((chars[3]));
                j = 3;
            }

            chars[j--] = *(string + i - 1);
        }

        // leftovers
        for (int8_t jcnt = 3; jcnt > j && k < 8; jcnt--)
        {
            key[k] |= (chars[jcnt]) << (24 - 8 * jcnt);
        }

        printf("Key was interpreted as string: ");
        for (size_t i = 8; i > 0; i--)
        {
            printf("%c%c%c%c", (key[i - 1] >> 24) & 0xFF, (key[i - 1] >> 16) & 0xFF, (key[i - 1] >> 8) & 0xFF, (key[i - 1] >> 0) & 0xFF);
        }
    }
    printf("\n");
}