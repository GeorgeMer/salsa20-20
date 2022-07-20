#include "file_IO.h"

const uint8_t *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    char *message = NULL;

    // error handling
    if (file == NULL)
    {
        perror("Error opening file");
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
    // manually add 0 byte
    message[statbuf.st_size] = '\0';

    size_t length = strlen(message);
    bool is_hex_string = true;

    // check if input file string is a valid hex number
    for (size_t i = 0; i < length; i++)
    {
        if (!((*(message + i) >= 0x30 && *(message + i) <= 0x39) || (*(message + i) >= 0x41 && *(message + i) <= 0x46) || (*(message + i) >= 0x61 && *(message + i) <= 0x66)))
        {
            is_hex_string = false;
            break;
        }
    }

    // if input file string is valid hex number, convert hexstring to byte array
    if (is_hex_string)
    {
        length = strlen(message) / 2 + 1;
        unsigned char *as_hex = malloc(length);
        char *save = message;
        for (size_t count = 0; count < length; count++)
        {
            sscanf(message, "%2hhx", (as_hex + count));
            message += 2;
        }
        *(as_hex + length - 1) = '\0';
        free(save);
        message = (char *)as_hex;
    }

    // close successfully read file, convert char array to uint8_t array
    fclose(file);
    mlen = length;
    return (const uint8_t *)message;

    // label to jump to if error occured reading input file
    // closes file and ends the program returning EXIT_FAILURE
error:
    fclose(file);
    exit(EXIT_FAILURE);
}

void write_file(const char *path, uint8_t *cipher)
{
    FILE *file;

    // error handling
    if (path == NULL)
    {
        // create txt file "result" if no path to output file was given
        if (!(file = fopen("result.txt", "w")))
        {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (!(file = fopen(path, "w")))
        {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
    }

    char *output_ascii = malloc(mlen);

    if (output_ascii != NULL)
    {
        // writing cipher to output file as a string of ascii characters
        for (size_t i = 0; i < mlen - 1; i++)
        {
            if (*(cipher + i) < 0x20 || *(cipher + i) > 0x7E)
            {
                free(output_ascii);
                goto hex;
            }
        }
        for (size_t i = 0; i < mlen - 1; i++)
        {
            if (fprintf(file, "%c", *(cipher + i)) < 0)
            {
                free(output_ascii);
                goto cleanup;
            }
        }
        free(output_ascii);
    }
    else
    {
    // cannot output cipher as ascii characters
    // writing cipher to output file as string of hexadecimals
    hex:;
        size_t linebreak = 0;
        for (size_t i = 0; i < mlen; i++)
        {
            // linebreak every 76 characters
            if (linebreak == 76)
            {
                if (fprintf(file, "\n") < 0)
                {
                    goto cleanup;
                }
                linebreak = 0;
            }

            if (fprintf(file, "%02x", *(cipher + i)) < 0)
            {
                goto cleanup;
            }
            linebreak += 2;
        }
    }
    // close successfully written file
    fclose(file);
    return;

cleanup:
    fprintf(stderr, "Error writing to output file\n");
    fclose(file);
    exit(EXIT_FAILURE);
}