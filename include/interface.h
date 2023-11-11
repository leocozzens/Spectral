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
typedef bool (*WinSetup)(void *window);

char *interface_init(void (*glfw_callback)(int, const char*));
bool interface_get_dim(Dimension *screenDim, char **errVal);
void interface_cut_dim(Dimension *targetDim, float portion, float scale, bool xScaled);
void *window_init(Dimension size, char *windowName, WinSetup win_setup, char **errVal);
int window_run(void *window, WinFunc win_cycle);
void interface_cleanup(void);

#endif