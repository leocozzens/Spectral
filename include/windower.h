#ifndef WINDOWER_H
#define WINDOWER_H

struct Result;
union Dimension;

typedef void (*WindowMain)(Dimension winDim, void *data);

struct Result windower_init(void);
void windower_cleanup(void);
Result window_create(Dimension windowDim, const char *windowName, WindowMain winMain, void *data);
Result windower_loop(void);

#endif