#include <sys/stat.h>
#include "file_IO.h"

uint8_t *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    char *message = NULL;

    // error handling
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fseek(file, 0, SEEK_END))
    {
        fprintf(stderr, "Error finding end of file\n");
        goto error;
    }

    long int a = ftell(file);
    if (a == -1)
    {
        fprintf(stderr, "Error in determining file lenght\n");
        goto error;
    }
    mlen = a;
    rewind(file);

    if (!(message = malloc(mlen * sizeof(char))))
    {
        fprintf(stderr, "Error reading file: Could not allocate enough memory\n");
        goto error;
    }

    if (fread(message, mlen, 1, file) != 1)
    {
        perror("Error reading file");
        goto error;
    }

    // close successfully read file, convert char array to uint8_t array
    fclose(file);
    return (uint8_t *)message;

    // label to jump to if error occured reading input file
    // closes file and ends the program returning EXIT_FAILURE
error:
    fclose(file);
    free(message);
    exit(EXIT_FAILURE);
}

void write_file(const char *path, uint8_t *cipher)
{
    FILE *file;

    // error handling
    if (path == NULL)
    {
        // create new binary file "encrypted.bin" if no path to output file was given
        if (!(file = fopen("encrypted.bin", "wb")))
        {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }
        if (chmod("encrypted.bin", 448) < 0)
        {

            fprintf(stderr, "Error changing file permissions\n");
            goto cleanup;
        }
    }
    else
    {
        if (!(file = fopen(path, "wb")))
        {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        if (chmod(path, 448) < 0)
        {

            fprintf(stderr, "Error changing file permissions\n");
            goto cleanup;
        }
    }

    if (fwrite(cipher, mlen, 1, file) != 1)
    {
        fprintf(stderr, "Error writing to output file\n");
        goto cleanup;
    }

    // close successfully written file
    fclose(file);
    return;

cleanup:
    fclose(file);
    exit(EXIT_FAILURE);
}