#include "file_IO.h"

const uint8_t *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    char *message = NULL;

    // error handling
    if (file == NULL)
    {
        perror("Error opening file ");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    if (fstat(fileno(file), &statbuf))
    {
        perror("Error retrieving file stats ");
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
        perror("Error reading file ");
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

void write_file(const char *path, uint8_t *cipher)
{
    FILE *file;

    // error handling
    if (path == NULL)
    {
        // create txt file "result" if no path to output file was given
        if (!(file = fopen("result.txt", "w")))
        {
            perror("Error creating file ");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (!(file = fopen(path, "w")))
        {
            perror("Error opening output file ");
            exit(EXIT_FAILURE);
        }
    }

    // // writing cipher to output file as string of hexadecimals
    // // linebreak every 76 characters
    size_t linebreak = 0;
    for (size_t i = 0; i < mlen; i++)
    {
        if (linebreak == 76)
        {
            if (fprintf(file, "\n") < 0)
            {
                goto cleanup;
            }
            linebreak = 0;
        }

        if (*(cipher + i) == 0)
        {
            // writing 0 to output file as 00
            if (fprintf(file, "00") < 0)
            {
                goto cleanup;
            }
        }
        else if (*(cipher + i) <= 15)
        {
            // writing single digit hex to output file as 0z
            if (fprintf(file, "0%x", *(cipher + i)) < 0)
            {
                goto cleanup;
            }
        }
        else
        {
            // writing double digit hex to output file
            if (fprintf(file, "%x", *(cipher + i)) < 0)
            {
                goto cleanup;
            }
        }
        linebreak += 2;
    }
    // close successfully written file
    // if (!fwrite((void *)cipher, 1, strlen((const char *)cipher), file))
    // {
    //     perror("Error writing to file");
    // }
    fclose(file);
    return;

cleanup:
    fprintf(stderr, "Error writing to output file\n");
    fclose(file);
    exit(EXIT_FAILURE);
}