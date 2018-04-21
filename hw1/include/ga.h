#ifndef __GA_H_
#define __GA_H_

#include "graph.h"

#define SEED_RATIO 0.3

#define FIT_MAX (1.50 / NUM_SOLUTIONS)
#define FIT_MIN (2.00 / NUM_SOLUTIONS - FIT_MAX)

#define DIST_MAX (MAX_VTX / 2)

#define XOVER_RATIO 0.5
#define MUTATION_RATIO 0.05

struct sol {
  unsigned char rep[MAX_VTX];
  int val;
};

unsigned char *best_solution();

void init_GA(int size);
int repeat_GA(double elapsed);

void crossover_GA(struct sol *gene, double decay);
void mutation_GA(struct sol *gene, double decay);
void replace_GA(struct sol *gene, int num_replace);

void pop_print();
void gen_print(int i);

#endif
