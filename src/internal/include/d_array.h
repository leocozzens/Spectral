#ifndef D_ARRAY_H
#define D_ARRAY_H

#include <stddef.h>

typedef struct {
    void *(*allocate)(size_t);
    void *(*reallocate)(void *, size_t);
    void  (*free)(void *);
} Allocator;

typedef struct {
    size_t len;
    size_t capacity;
    size_t growthFactor;
    Allocator *arrayAllocator;
} D_ArrayMeta;

#define DEFAULT_INIT_CAPACITY   16

#define D_META(_arr)            ((D_ArrayMeta*)(_arr) - 1)
#define D_ARRAY_LEN(_arr)       D_META(_arr)->len
#define D_ARRAY_CAPACITY(_arr)  D_META(_arr)->capacity

#define D_ARRAY_CREATE(_type, _size) \
    d_create_array(sizeof(_type), _size, _Alignof(_type), NULL)
#define D_ARRAY_CREATE_ALLOC(_type, _size, _alloc) \
    d_create_array(sizeof(_type), _size, _Alignof(_type), _alloc)

#define D_ARRAY_NEXT(_arr)          (_arr)[D_ARRAY_LEN(_arr)++]
#define D_ARRAY_CURRENT(_arr)       (_arr)[D_ARRAY_LEN(_arr)]

#define D_ARRAY_APPEND(_arr, _arg) \
    { \
        void *tmp = d_check_array(_arr, 1, sizeof(_arg)), \
        if(tmp != NULL) { \
            _arr = tmp; \
            D_ARRAY_NEXT(_arr) = _arg; \
        } \
    }

#define D_SHRINK_ARRAY(_arr)            (_arr = d_array_shrink(_arr))
#define D_POP_BACK(_arr)                (D_ARRAY_LEN(_arr)--)
#define D_ARRAY_REMOVE(_arr, _index)    (d_array_remove(_arr, _index, sizeof(*_arr)))

void *d_create_array(size_t elementSize, size_t elements, size_t alignment, Allocator *arrAllocator);
void *d_check_array(void *arr, size_t newItems, size_t itemSize);
void d_array_remove(void *arr, size_t index, size_t itemSize);
void d_destroy_array(void **arr);

extern Allocator DEFAULT_ALLOCATOR;

#endif