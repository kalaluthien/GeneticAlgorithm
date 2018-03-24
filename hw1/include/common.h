#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#define MAX_TIME 120.0

#define throw(msg) \
  do { \
    printf("[%s:%d] Error: %s\n", __FILE__, __LINE__, (msg)); \
    exit(EXIT_FAILURE); \
  } while (0)

#endif
