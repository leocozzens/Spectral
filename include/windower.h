#ifndef WINDOWER_H
#define WINDOWER_H

struct Result;
union Dimension;

struct Result windower_init(void);
Result window_create(void **window, union Dimension windowDim, const char *windowName);

#endif