#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_line(const char *line, size_t length, size_t columns) {
    if (printf("%4zu: ", columns) < 0) {
        return -1;
    }

    if (length > 0 && fwrite(line, 1, length, stdout) != length) {
        return -1;
    }

    return 0;
}

int main(void) {
    size_t capacity = 256;
    size_t length = 0;
    size_t columns = 0;
    char *line = malloc(capacity);
    int character;

    if (line == NULL) {
        fputs("linecheck: unable to allocate input buffer\n", stderr);
        return EXIT_FAILURE;
    }

    while ((character = fgetc(stdin)) != EOF) {
        if (length == capacity) {
            size_t new_capacity;
            char *resized_line;

            if (capacity > SIZE_MAX / 2) {
                fputs("linecheck: input line is too long\n", stderr);
                free(line);
                return EXIT_FAILURE;
            }

            new_capacity = capacity * 2;
            resized_line = realloc(line, new_capacity);
            if (resized_line == NULL) {
                fputs("linecheck: unable to grow input buffer\n", stderr);
                free(line);
                return EXIT_FAILURE;
            }

            line = resized_line;
            capacity = new_capacity;
        }

        line[length++] = (char)character;

        if (character == '\n') {
            if (print_line(line, length, columns) != 0) {
                fputs("linecheck: output error\n", stderr);
                free(line);
                return EXIT_FAILURE;
            }

            length = 0;
            columns = 0;
        } else if (character == '\t') {
            /* Tabs advance to the next eight-column stop. */
            columns += 8 - (columns % 8);
        } else {
            columns++;
        }
    }

    if (ferror(stdin)) {
        fputs("linecheck: input error\n", stderr);
        free(line);
        return EXIT_FAILURE;
    }

    if (length > 0 && print_line(line, length, columns) != 0) {
        fputs("linecheck: output error\n", stderr);
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}
