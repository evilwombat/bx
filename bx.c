/* Copyright (c) 2020 evilwombat
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void show_help(const char *name)
{
    printf("%s input_file offset length output_file\n", name);
    printf("\toffset and length are in bytes, and a prefix of 0x means hex\n");
    printf("\tA length of + means dump from offset to end of input file\n");
}

int main(int argc, char **argv)
{
    size_t offset = 0, length = 0;
    size_t cur = 0, left = 0, copy_size = 0;
    ssize_t read_size = 0;
    const char *input_filename, *output_filename;
    FILE *src;
    FILE *dst;
    struct stat st;

    uint8_t copy_buffer[BUFFER_SIZE];

    if (argc < 5) {
        show_help(argv[0]);
        return 1;
    }

    input_filename = argv[1];
    offset = strtol(argv[2], NULL, 0);
    output_filename = argv[4];

    if (stat(input_filename, &st)) {
        fprintf(stderr, "Could not get size of %s: ", input_filename);
        perror(NULL);
        return 1;
    }

    if (offset > st.st_size) {
        fprintf(stderr, "Offset %zu exceeds length of %s\n", offset, input_filename);
        return 1;
    }


    if (strcmp(argv[3], "+") != 0) {
        length = strtol(argv[3], NULL, 0);
    } else {
        length = st.st_size - offset;
    }

    src = fopen(input_filename, "rb");
    dst = fopen(output_filename, "wb");

    if (!src) {
        fprintf(stderr, "Could not open %s for reading: ", input_filename);
        perror(NULL);
        return 1;
    }

    if (!dst) {
        fprintf(stderr, "Could not open %s for writing: ", output_filename);
        perror(NULL);
        return 1;
    }

    if (fseek(src, (long) offset, SEEK_SET)) {
        fprintf(stderr, "Could not seek to position %zu\n", offset);
        return 1;
    }

    left = length;
    while (left) {
        if (left > BUFFER_SIZE)
            copy_size = BUFFER_SIZE;
        else
            copy_size = left;

        read_size = fread(copy_buffer, 1, copy_size, src);

        if (read_size <= 0) {
            fprintf(stderr, "Error reading from %s: ", input_filename);
            perror(NULL);
            return 1;
        }

        if (fwrite(copy_buffer, 1, read_size, dst) != read_size) {
            fprintf(stderr, "Error writing to %s: ", output_filename);
            perror(NULL);
            return 1;
        }

        if (read_size < copy_size) {
            fprintf(stderr, "Warning: short read while reading %s\n", input_filename);
            return 0;
        }

        left -= read_size;
    }

    fclose(src);
    fclose(dst);
    return 0;
}
