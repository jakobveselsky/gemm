#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
    float value;
} entry_t;

typedef struct{
    entry_t** entries;
    int size;
    int head;
    int tail;
    pthread_mutex_t lock;
    pthread_cond_t has_space;
    pthread_cond_t has_items;
} buffer_t;

extern void buffer_init(buffer_t* b, int N);
extern void buffer_put(buffer_t* b, entry_t* item);
extern entry_t* buffer_get(buffer_t* b);
extern int buffer_count(buffer_t* b);
extern void buffer_cleanup(buffer_t* b);

#endif