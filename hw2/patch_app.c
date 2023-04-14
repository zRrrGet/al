#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    // Open the file for reading
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long int file_size = ftell(file);
    rewind(file);

    // Allocate memory for the file contents
    char *file_contents = malloc(file_size);
    if (file_contents == NULL) {
        printf("Error: could not allocate memory\n");
        return 1;
    }

    // Read the file contents into memory
    size_t bytes_read = fread(file_contents, 1, file_size, file);
    if (bytes_read != file_size) {
        printf("Error: could not read file %s\n", filename);
        return 1;
    }

    // Change bytes
    if (file_size < 0x15e7+6) {
        printf("Error: file %s does not have 0x15e7+6 bytes\n", filename);
        return 1;
    }
    file_contents[0x15e7] = 0x48;
    file_contents[0x15e7+1] = 0x8d;
    file_contents[0x15e7+2] = 0x35;
    file_contents[0x15e7+3] = 0x52;
    file_contents[0x15e7+4] = 0x3a;
    file_contents[0x15e7+5] = 0;
    file_contents[0x15e7+6] = 0;

    // Construct the output filename
    char *output_filename = malloc(strlen(filename) + strlen("_patched") + 1);
    if (output_filename == NULL) {
        printf("Error: could not allocate memory\n");
        return 1;
    }
    sprintf(output_filename, "%s_patched", filename);

    // Open the output file for writing
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        printf("Error: could not open file %s for writing\n", output_filename);
        return 1;
    }

    // Write the modified file contents to the output file
    size_t bytes_written = fwrite(file_contents, 1, file_size, output_file);
    if (bytes_written != file_size) {
        printf("Error: could not write file %s\n", output_filename);
        return 1;
    }

    // Clean up
    fclose(file);
    fclose(output_file);
    free(file_contents);
    free(output_filename);

    return 0;
}