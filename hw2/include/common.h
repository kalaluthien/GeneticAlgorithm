#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#define LOCAL_OPTIMIZE

#define NUM_SOLUTIONS 128
#define NUM_PROFILING 64
#define NUM_SURVIVORS 8
#define NUM_OFFSPRING 1

#define QNUM 8

#define PROF_COUNT 8
#define UNIT_TIME 10.0
#define TOTAL_TIME 180.0

#define throw(cond, msg) \
  do { \
    if (cond) { \
      printf("[%s:%d] Exception: %s\n", __FILE__, __LINE__, (msg)); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#endif
