#include <stdio.h>
#include <stdlib.h>

char* long_to_string(long number) {
    // Calculate how many bytes i need to store the number
    int chars_needed = snprintf(NULL, 0, "%ld", number);
    if (chars_needed < 0) {
        return NULL; 
    }

    // Chars + '\0'
    size_t required_size = chars_needed + 1;

    // Allcoate memeory
    char* result = (char*)malloc(required_size);
    if (result == NULL) {
        return NULL; 
    }

    snprintf(result, required_size, "%ld", number);
    return result;
}