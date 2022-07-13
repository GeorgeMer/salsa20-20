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
        printf("Key was interpreted as hex: ");
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
                fprintf(stderr, "The key entered does not fit in 256 bit.\n");
                exit(EXIT_FAILURE);
            }
        }
        string += start;
        size_t i = strlen(string) - 1, k = 0;
        for (; i >= 8; i -= 8, k++)
        {
            char hexnum[8] = {*(string + i - 7), *(string + i - 6), *(string + i - 5), *(string + i - 4),
                              *(string + i - 3), *(string + i - 2), *(string + i - 1), *(string + i)};
            key[k] = convert_string_to_uint32_t(hexnum, base);
        }

        char *lastnum = malloc(i + 2);
        if (i != 0)
        {
            if (lastnum == NULL)
            {
                fprintf(stderr, "Couldn't allocate memory\n");
                exit(EXIT_FAILURE);
            }
            for (size_t j = 0; j <= i; j++)
            {
                *(lastnum + j) = *(string + j);
            }
            *(lastnum + i + 1) = '\0';
            key[k] = convert_string_to_uint32_t(lastnum, base);

            free(lastnum);
        }
        else
        {
            if (lastnum == NULL)
            {
                fprintf(stderr, "Couldn't allocate memory\n");
                exit(EXIT_FAILURE);
            }
            *lastnum = *string;
            *(lastnum + 1) = '\0';
            key[k] = convert_string_to_uint32_t(lastnum, base);
            free(lastnum);
        }

        printf("0x");
        for (size_t i = 0; i < 8; i++)
        {
            printf("%08x", key[i]);
        }
    }

    // key is interpreted as a string
    /*
        (eg: "HelloWorld" will be interpreted as
        key[7] = 48 65 6C 6C
        key[6] = 6F 57 6F 72
        key[5] = 6C 64 00 00
        key 4 to 0 will be padded with 0s
    */
    else
    {
        printf("Key was interpreted as string: ");
        uint8_t chars[4];
        size_t i = 0, j = 0, k = 8;
        for (; i < strlen(string) && k > 0; i++, j++)
        {

            // every four bytes convert to a key element ()
            if (j == 4)
            {
                key[k - 1] = ((chars[0]) << 24) | ((chars[1]) << 16) | ((chars[2]) << 8) | ((chars[3]));

                k--;
                j = 0;
            }

            chars[j] = *(string + i);
        }

        for (size_t jcnt = 0; jcnt < j && k > 0; jcnt++)
        {
            key[k - 1] |= (chars[jcnt]) << (24 - 8 * jcnt);
        }

        for (; j < 4 && k > 0; j++)
        {
            key[k - 1] |= (0) << (24 - 8 * j);
        }

        if (k != 0)
            k--;

        for (; k > 0; k--)
        {
            key[k - 1] = 0;
        }

        for (i = 8; i > 0; i--)
        {
            printf("%c%c%c%c", (key[i - 1] >> 24) & 0xFF, (key[i - 1] >> 16) & 0xFF, (key[i - 1] >> 8) & 0xFF, (key[i - 1] >> 0) & 0xFF);
        }
    }

    printf("\n\n");
}