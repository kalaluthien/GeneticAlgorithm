#ifndef __GA_H_
#define __GA_H_

#include "graph.h"

struct sol {
  int value[MAXVTX];
};

void init_GA(struct graph *g, int num_sols);
int repeat_GA();
void crossover_GA(struct sol *offspring);
void mutation_GA(struct sol *offspring);
void replace_GA(struct sol *offspring);

#endif
