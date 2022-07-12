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

struct Thread_info {
   int do_mult;
   int index;
   int N;
};

#endif
