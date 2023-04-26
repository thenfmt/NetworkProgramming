#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_system.h"

int file_formatting(char *file_path, char **buffer) {
    long filelen;
    int filename_len;
    char *filename;

    
    // Remove the newline character at the end of the input
    size_t len = strlen(file_path);
    if (len > 0 && file_path[len-1] == '\n') {
        file_path[len-1] = '\0';
    }
    
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        // Handling errors when the file cannot be opened
        printf("\nError: Cannot open file");
        return 0;
    }

    // Extract filename and extension
    filename = strrchr(file_path, '/');
    if (filename == NULL) {
        filename = file_path;
    } else {
        filename = filename + 1;
    }

    // Get the length of the file
    fseek(file, 0, SEEK_END);
    filelen = ftell(file);
    rewind(file);

    // Allocate memory for the buffer
    (*buffer) = (char *)malloc((filelen+1)*sizeof(char));

    // Insert filename and extension at the beginning of the (*buffer)
    sprintf((*buffer), "%s\n", filename);

    // Read the contents of the file into the (*buffer)
    fread((*buffer) + strlen((*buffer)), filelen, 1, file);

    fclose(file);

    return 1;
}


int create_file_with_format(char *buffer) {
    char *filename, *text;
    FILE *file;

    // Extract filename and text from buffer
    filename = strtok(buffer, "\n");
    text = strtok(NULL, "");

    // Open file for writing
    file = fopen(filename, "wb");
    if (file == NULL) {
        // Handling errors when the file cannot be opened
        printf("\nError: Cannot open file for writing.");
        return 0;
    }

    // Write text to file
    fprintf(file, "%s", text);

    // Close file
    fclose(file);

    return 1;
}
