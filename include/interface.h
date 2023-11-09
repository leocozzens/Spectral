#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

typedef union {
    struct {
        int x;
        int y;
    };
    struct {
        int width;
        int height;
    };
    int unit[2];
} Dimension;

typedef bool (*WinFunc)(void *window, int *exitVal);

char *interface_init(void (*glfw_callback)(int, const char*));
bool interface_get_dim(Dimension *screenDim, char **errVal);
void *window_init(Dimension size, char *windowName, char **errVal);
int window_run(void *window, WinFunc win_main);
void interface_cleanup(void);

#endif