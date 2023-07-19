#include "main.h"

#define BUFFER_SIZE 1024

ssize_t custom_getline(char **lineptr, size_t *n) {
    static char buffer[BUFFER_SIZE];
    static size_t buffer_pos = 0;
    static ssize_t bytes_read = 0;
    ssize_t i = 0; j = 0;
    ssize_t bytes_to_copy = 0;
    char *new_lineptr;

    if (*lineptr == NULL || *n == 0) {
        *n = BUFFER_SIZE;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return (-1);
        }
    }

    while (1) {
        if (buffer_pos >= bytes_read) {
            bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            buffer_pos = 0;
            if (bytes_read <= 0) {
                break;
            }
        }

        bytes_to_copy = bytes_read - buffer_pos;

        while (i < bytes_to_copy) {
            if (buffer[buffer_pos + i] == '\n') {
                (*lineptr)[i] = '\0';
                buffer_pos += (i + 1);
                return (i);
            }
            i++;
        }

        /* Reallocate the buffer if needed */
        if ((*n - 1) <= i) {
            *n *= 2;
            *new_lineptr = realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                return (-1);
            }
            *lineptr = new_lineptr;
        }

        /* Copy the characters from the buffer to the lineptr */
        for (j = 0; j < bytes_to_copy; j++) {
            (*lineptr)[i++] = buffer[buffer_pos++];
        }
    }

    /* Reached end of file or encountered an error */
    if (i > 0) {
        (*lineptr)[i] = '\0';
        return (i);
    }

    return (-1); /* Error reading from stdin */
}
