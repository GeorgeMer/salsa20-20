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

        size_t j = 0, k = 0;
        size_t length = strlen(string);
        char hexnum[9];

        for (size_t i = 0; i < length; i++, j++)
        {
            // every 8 characters of the string are converted to one 32 bit number and written into the key array
            if (j == 8)
            {
                hexnum[8] = '\0';
                key[k++] = convert_string_to_uint32_t(hexnum, base);
                j = 0;
            }

            hexnum[j] = *(string + i);
        }

        for (; j <= 8 && j > 0; j++)
        {
            if (j == 8)
            {
                hexnum[8] = '\0';
                key[k] = convert_string_to_uint32_t(hexnum, base);
                break;
            }

            hexnum[j] = '0';
        }
    }

    // key is interpreted as a string
    /*
        (eg: "HelloWorld" will be interpreted as
        key[0] = 48 65 6C 6C
        key[1] = 6F 57 6F 72
        key[2] = 6C 64 00 00
        key 3 to 7 will be padded with 0s
    */
    else
    {
        uint8_t chars[4];
        size_t j = 0, k = 0;
        for (size_t i = 0; i < strlen(string) && k < 8; i++)
        {
            // every four bytes convert to a key element
            if (j == 4)
            {
                key[k++] = ((chars[0]) << 24) | ((chars[1]) << 16) | ((chars[2]) << 8) | ((chars[3]));
                j = 0;
            }

            chars[j++] = *(string + i);
        }

        for (size_t jcnt = 0; jcnt < j && k < 8; jcnt++)
        {
            key[k] |= (chars[jcnt]) << (24 - 8 * jcnt);
        }
    }
}