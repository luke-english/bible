#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read an entire file into a string
char* read_file_to_string(const char* filename) {
  // Open the file for reading
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    // Return NULL if the file cannot be opened
    return NULL;
  }

  // Get the size of the file
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);

  // Rewind the file back to the beginning
  rewind(fp);

  // Allocate a buffer to hold the file contents
  char* buffer = (char*)malloc(size + 1);
  if (buffer == NULL) {
    // Return NULL if the buffer cannot be allocated
    fclose(fp);
    return NULL;
  }

  // Read the file into the buffer
  size_t bytes_read = fread(buffer, sizeof(char), size, fp);
  if (bytes_read != size) {
    // Return NULL if the file cannot be read
    fclose(fp);
    free(buffer);
    return NULL;
  }

  // Close the file and null-terminate the buffer
  fclose(fp);
  buffer[size] = '\0';

  // Return the buffer
  return buffer;
}
