#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define INIT_LIST(_obj, _initSize, _elemSize)   _obj.capacity = _initSize; \
                                                _obj.count = 0; \
                                                _obj.list = data_array_init((_initSize), (_elemSize))
#define ADDTO_LIST(_list, _newItem)             data_array_add((void**) &(_list.list), (const void*) &(_newItem), &(_list.count), &(_list.capacity), sizeof(_newItem))
#define KILL_LIST(_list)                        data_array_kill((void**) &(_list.list), &(_list.count), &(_list.capacity))

#define VERTEX_ELEM                             3

typedef union {
    struct {
        float x;
        float y;
        float z;
    };
    float pos[VERTEX_ELEM];
} Vertex;

typedef struct _DrawDetails {
    unsigned int VAO, numElems;
} DrawDetails;

typedef struct _VertexList {
    Vertex *list;
    unsigned int count;
    unsigned int capacity;
} VertexList;

typedef struct _ElemList {
    unsigned int *list;
    unsigned int count;
    unsigned int capacity;
} ElemList;

typedef struct _DrawList {
    DrawDetails *list;
    unsigned int count;
    unsigned int capacity;
} DrawList;

void *data_array_init(unsigned int initSize, unsigned int elemSize);
bool data_array_add(void **arr, const void *newItem, unsigned int *size, unsigned int *capacity, unsigned int elemSize);
void data_array_set(void *arr, unsigned int index, unsigned int elemSize, void *data);
void data_array_kill(void **arr, unsigned int *size, unsigned int *capacity);

#endif