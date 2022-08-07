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
    buffer_t* input_vector;
    buffer_t* output_vector;
    buffer_t* scalers_list;
    int index;
    int N;
} b_tsd_t;

#endif
