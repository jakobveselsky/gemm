#ifndef _THREADS_H
#define _THREADS_H

//define included libaries
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/queue.h>
#include <getopt.h>

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

typedef struct{
    buffer_t* b;
    int index;
    int N;
} b_tsd_t;

#endif
