#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#define NUM_PHASE 5
#define TIME_LIMIT 500.0
#define TIME_PER_PHASE (TIME_LIMIT / NUM_PHASE - 1.0)

#define NUM_SOLUTION 64

#define QNUM 8

#define throw(cond, msg) \
  do { \
    if (cond) { \
      printf("[%s:%d] Exception: %s\n", __FILE__, __LINE__, (msg)); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#endif
