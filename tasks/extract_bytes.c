/*
ExByte - Byte to Literal Extraction Tool
Author: Leo Cozzens
Date: 11/12/2023

Description:
Generalist tool made to extract text data from ASCII source files.
Outputs into a single compiler readable string literal at the runtime specified output.


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files.
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

// C standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_NAME    "exlit"
#define USAGE           "%s [SOURCE FILE] [TARGET FILE]"
#define READ_MODE       "r"
#define WRITE_MODE      "w"
#define SOURCE_ERR      "Failed to open source file - %s\n"
#define TARGET_ERR      "Failed to open target file - %s\n"
#define END_OF_LINE     "\"\n\""
#define QUOTE           '\"'

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define LINE_SIZE       20

#define CHECK_SERVICE(_serv, _fmt)  if(_serv == NULL) { fprintf(stderr, _fmt, strerror(errno)); return EXIT_FAILURE; } 

static char *extract_name(char *filePath) {
    char *lastSlash = strrchr(filePath, '/');
    if(lastSlash == NULL) lastSlash = strrchr(filePath, '\\');
    return (lastSlash == NULL) ? filePath : lastSlash + 1;
}

static unsigned char *read_file(FILE *target, long *fSize) {
    if(target == NULL) return NULL;
    fseek(target, 0, SEEK_END);
    *fSize = ftell(target);
    fseek(target, 0, SEEK_SET);

    unsigned char *store = malloc(*fSize + 1);
    if(store == NULL) return NULL;
    fread(store, sizeof(char), *fSize, target);
    store[*fSize] = '\0';
    return store;
}

int main(int argc, char **argv) {
    if(argc != 3) {
        printf(USAGE, (argv[0] == NULL) ? DEFAULT_NAME : extract_name(argv[0]));
        return EXIT_FAILURE;
    }

    FILE *target = fopen(argv[2], WRITE_MODE);
    CHECK_SERVICE(target, TARGET_ERR);
    FILE *source = fopen(argv[1], READ_MODE);  // Close last file
    CHECK_SERVICE(source, SOURCE_ERR);

    long fSize;
    unsigned char *buffer = read_file(source, &fSize);
    fputc(QUOTE, target);
    for(int i = 0; i < fSize; i++) {
        fprintf(target, "\\x%02x", buffer[i]);
        if((i + 1) % LINE_SIZE == 0 && i < fSize - 1) 
            fputs(END_OF_LINE, target);
    }
    free(buffer);
    fputc(QUOTE, target);

    fclose(target);
    fclose(source);
    return EXIT_SUCCESS;
}