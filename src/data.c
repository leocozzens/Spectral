// C standard headers
#include <stdlib.h>
#include <stdbool.h>

#define GROWTH_FACTOR   2

// Local function prototypes
static void *local_memcpy(void *dest, const void *src, unsigned int n);

// Public functions
void *data_array_init(unsigned int initSize, unsigned int elemSize) {
    return malloc(initSize * elemSize);
}

bool data_array_add(void **arr, const void *newItem, unsigned int *size, unsigned int *capacity, unsigned int elemSize) {
    if(arr == NULL || *arr == NULL || newItem == NULL) return true;
    if(*size >= *capacity) {
        void *tmp = realloc(*arr, (*capacity * GROWTH_FACTOR) * elemSize);
        if(tmp == NULL) return true;
        *arr = tmp;
        *capacity *= 2;
    }

    local_memcpy(((char*) *arr) + ((*size)++ * elemSize), newItem, elemSize);
    return false;
}

void data_array_set(void *arr, unsigned int index, unsigned int elemSize, void *data) {
    local_memcpy(((char*) arr) + index * elemSize, data, elemSize);
}

void data_array_kill(void **arr, unsigned int *size, unsigned int *capacity) {
    free(*arr);
    *arr = NULL;
    *size = 0;
    *capacity = 0;
}

// Local function definitions
static void *local_memcpy(void *dest, const void *src, unsigned int n) {
    char *cDest = dest;
    const char *cSrc = src;

    for(unsigned int i = 0; i < n; i++) {
        *cDest++ = *cSrc++;
    }
    return dest;
}