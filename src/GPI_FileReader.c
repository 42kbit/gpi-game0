#include "GPI_FileReader.h"

#include <malloc.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define GPI_MAX_FILE_BUFFER_CAPACITY 8192

char* GPI_ReadFile(char* filePath) // dont forget to free(void*);
{
    char* content = (char*)malloc(sizeof(char) * GPI_MAX_FILE_BUFFER_CAPACITY);
    memset(content, 0, sizeof(*content) * GPI_MAX_FILE_BUFFER_CAPACITY);
    uint32_t bufferTop = 0;
    char c;
    FILE* file = fopen(filePath, "r");
        while((c = fgetc(file)) != EOF)
            content[bufferTop++] = c;
    fclose(file);

    return content;
}
