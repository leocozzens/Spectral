// Local headers
#include <dimension.h>

Dimension EMPTY_DIM = { 0, 0 };

Dimension create_dimension(int x, int y) {
    Dimension newDim = { x, y };
    return newDim;
}