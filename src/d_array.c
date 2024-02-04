// C standard headers
#include <stdlib.h>
#include <string.h>
// Local headers
#include <d_array.h>

Allocator DEFAULT_ALLOCATOR = { malloc, realloc, free };

void *d_create_array(size_t elementSize, size_t elements, size_t alignment, Allocator *arrAllocator) {
    if(arrAllocator == NULL) arrAllocator = &DEFAULT_ALLOCATOR;

    size_t initSize = sizeof(D_ArrayMeta) + elements * elementSize;
    D_ArrayMeta *newArr = arrAllocator->allocate(initSize);
    if(newArr != NULL) {
        newArr->arrayAllocator = arrAllocator;
        newArr->capacity = initSize;
        newArr->growthFactor = 2;
        newArr->len = 0;
        newArr++;
    }
    return newArr;
}

void *d_check_array(void *arr, size_t newItems, size_t itemSize) {
    D_ArrayMeta *arrMeta = D_META(arr);
    size_t potentialSize = arrMeta->len + newItems;

    if(arrMeta->capacity < potentialSize) {
        size_t newSize = arrMeta->capacity;
        do {
            newSize *= arrMeta->growthFactor;
        } while(newSize < potentialSize);

        arrMeta = arrMeta->arrayAllocator->reallocate(arr, newSize * itemSize);
        if(arrMeta == NULL) return NULL;
        arrMeta->capacity = newSize;
    }
    return arrMeta + 1;
}

void *d_shrink_array(void *arr, size_t itemSize) {
    D_ArrayMeta *arrMeta = D_META(arr);

    size_t newSize = arrMeta->len;
    arrMeta = arrMeta->arrayAllocator->reallocate(arr, arrMeta->len * itemSize);
    if(arrMeta == NULL) return arr;

    arrMeta->capacity = arrMeta->len;
    return arrMeta + 1;
}

void *d_set_array_size(void *arr, size_t newSize, size_t itemSize) {
    D_ArrayMeta *arrMeta = D_META(arr);

    arrMeta = arrMeta->arrayAllocator->reallocate(arr, newSize * itemSize);
    if(arrMeta == NULL) return NULL;

    arrMeta->capacity = newSize;
    if(arrMeta->len > newSize) arrMeta->len = newSize;
    return arrMeta + 1;
}

// void ordered_remove

void d_array_remove(void *arr, size_t index, size_t itemSize) {
    D_ArrayMeta *arrMeta = D_META(arr);
    if(arrMeta->len == 0) return;

    if(index == arrMeta->len - 1) {
        arrMeta->len--;
        return;
    }
    void *curr = (char*) arr + index;
    void *last = (char*) arr + arrMeta->len;
    arrMeta->len--;
    memcpy(curr, last, itemSize);
}

void d_destroy_array(void **arr) {
    if(arr == NULL || *arr == NULL) return;
    D_META(*arr)->arrayAllocator->free(D_META(*arr));
    *arr = NULL;
}