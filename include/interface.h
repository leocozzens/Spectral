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

typedef bool (*WinMaster)(void *window, int *exitVal);
typedef bool (*WinSetup)(void *window);
typedef void (*WinCleanup)(void *window);
typedef void (*ErrorCallback)(int, const char*);
typedef struct GLFWwindow GLFWwindow;

typedef struct {
    void (*key_func)(GLFWwindow *, int, int, int, int);
    void (*char_func)(GLFWwindow *, unsigned int);
    void (*size_func)(GLFWwindow *, int, int);
    void (*cursor_pos_func)(GLFWwindow *, double, double);
    void (*cursor_enter_func)(GLFWwindow *, int);
    void (*scroll_func)(GLFWwindow *, double, double);
    void (*close_func)(GLFWwindow *);
} WinCallbacks;

typedef struct {
    WinMaster cycle_win;
    WinSetup setup_win;
    WinCleanup cleanup_win;
} WinTools;

char *interface_init(ErrorCallback error_callback);
bool interface_get_dim(Dimension *screenDim, char **errVal);
void interface_cut_dim(Dimension *targetDim, float portion, float scale, bool xScaled);
void *interface_window_init(Dimension size, char *windowName, WinTools *winToolset, WinCallbacks *callbacks, char **errVal);
int interface_window_run(void *window);
void interface_cleanup(void);

#endif