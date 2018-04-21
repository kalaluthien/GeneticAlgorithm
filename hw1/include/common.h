#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#define NUM_SOLUTIONS 256
#define NUM_OFFSPRING 8

#define QNUM 8
#define QVAL (NUM_SOLUTIONS / QNUM)

#define MAX_TIME 150.0

#define throw(cond, msg) \
  do { \
    if (cond) { \
      printf("[%s:%d] Exception: %s\n", __FILE__, __LINE__, (msg)); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#endif
