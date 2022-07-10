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
    if (*(string) == '-')
    {
        fprintf(stderr, "Negative numbers are not allowed as options.\n");
        exit(EXIT_FAILURE);
    }

    // converting 256 bit string input number into 8 element uint_32t array
    uint8_t index = 0;
    uint8_t count = 0;
    char current_string[9];
    int base = 10;

    if (*(string) == '0' && *(string + 1) == 'x')
    {
        base = 16;
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
        if (i != 0)
        {
            char *lastnum = malloc(i + 2);
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

            char *lastnum = malloc(2);
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
        for (int i = 0; i < 8; i++)
        {
            printf("%08x ", key[i]);
        }
    }

    else
    {

        if (strlen(string) > 77)
        {
            fprintf(stderr, "The key entered does not fit in 256 bit.\n");
            exit(EXIT_FAILURE);
        }
    }

    while (*(string) != '\0')
    {
        current_string[count++] = *(string);
        string += 1;
        if (count == 8)
        {
            current_string[count] = '\0';
            count = 0;
            key[index++] = convert_string_to_uint32_t(current_string, base);
        }
    }

    if (count != 0)
    {
        while (count < 8)
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
}