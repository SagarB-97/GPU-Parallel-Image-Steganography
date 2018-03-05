#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *data;
    int size;
} MP3File;

MP3File *readMP3(const char *filename)
{

    char *buffer;

    FILE *pFile = fopen(filename, "rb");

    // obtain file size
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);

    buffer = (char *)malloc(sizeof(char) * lSize);
    size_t result = fread(buffer, 1, lSize, pFile);

    MP3File *newFile = (MP3File *)malloc(sizeof(MP3File));
    newFile->data = buffer;
    newFile->size = lSize;

    fclose(pFile);

    return newFile;
}

void writeMP3(const char *filename, char *data, size_t size)
{

    FILE *file = fopen(filename, "wb");
    fwrite(data, 1, size, file);
    fclose(file);
}