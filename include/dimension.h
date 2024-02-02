#ifndef DIMENSION_H
#define DIMENSION_H

 typedef union Dimension {
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

Dimension create_dimension(int x, int y);

extern Dimension EMPTY_DIM;

#endif