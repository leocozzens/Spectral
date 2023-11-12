/*
ExLit - Literal Extraction Tool
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
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_NAME    "exlit"
#define USAGE           "%s [SOURCE FILE] [TARGET FILE]"
#define READ_MODE       "r"
#define WRITE_MODE      "w"
#define SOURCE_ERR      "Failed to open source file - %s\n"
#define TARGET_ERR      "Failed to open target file - %s\n"
#define END_OF_LINE     " \\n\"\n"
#define QUOTE           '\"'

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define BUFF_SIZE       256

#define CHECK_SERVICE(_serv, _fmt)  if(_serv == NULL) { fprintf(stderr, _fmt, strerror(errno)); return EXIT_FAILURE; } 

static char *extract_name(char *filePath) {
    char *lastSlash = strrchr(filePath, '/');
    if(lastSlash == NULL) lastSlash = strrchr(filePath, '\\');
    return (lastSlash == NULL) ? filePath : lastSlash + 1;
}

static bool check_endl(char *str) {
    char *endL;

    endL = strchr(str, '\r');
    if(endL != NULL) {
        *endL = '\0';
        return true;
    }
    endL = strchr(str, '\n');
    if(endL != NULL) {
        *endL = '\0';
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
    if(argc != 3) {
        printf(USAGE, (argv[0] == NULL) ? DEFAULT_NAME : extract_name(argv[0]));
        return EXIT_FAILURE;
    }

    FILE *source = fopen(argv[1], READ_MODE);
    CHECK_SERVICE(source, SOURCE_ERR);
    FILE *target = fopen(argv[2], WRITE_MODE);
    CHECK_SERVICE(target, TARGET_ERR);

    char buffer[BUFF_SIZE];
    bool firstRead = true;
    while(fgets(buffer, BUFF_SIZE, source)) {
        if(firstRead) {
            fputc(QUOTE, target);
            firstRead = false;
        }

        bool completeLine = check_endl(buffer);
        fputs(buffer, target);

        if(completeLine) {
            fputs(END_OF_LINE, target);
            firstRead = true;
        }
    }
    fputc(QUOTE, target);

    return EXIT_SUCCESS;
}