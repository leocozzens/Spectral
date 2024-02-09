#ifndef WINDOWER_H
#define WINDOWER_H

struct Result;
union Dimension;

typedef struct Result (*WindowSetup)(void *window, void **data);
typedef struct Result (*WindowDraw)(void *window, void *data);
typedef void (*WindowCleanup)(void *window, void *data);

typedef struct {
    WindowSetup setup_win;
    WindowDraw cycle_win;
    WindowCleanup cleanup_win;
} WinTools;

struct Result windower_init(void);
void windower_cleanup(void);
Result window_create(union Dimension windowDim, const char *windowName, WinTools toolSet, void *data);
void windower_loop(void);

#endif