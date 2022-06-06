#ifndef _THREADSIMD_H
#define _THREADSIMD_H

//define included libaries
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>

struct Thread_simd {
   int do_mult;
   int index;
};

#endif
