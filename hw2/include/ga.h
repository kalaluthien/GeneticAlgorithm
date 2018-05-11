#ifndef __GA_H_
#define __GA_H_

#include "graph.h"

#define FIT_MAX (1.50 / NUM_SOLUTIONS)
#define FIT_MIN (2.00 / NUM_SOLUTIONS - FIT_MAX)

#define MUTATION_RATIO 0.10

struct sol {
  int rep[MAX_VTX];
  int val;
};

struct sol *new_sol();
struct sol *best_sol();
struct sol *next_sol(int val);
struct sol *clone_sol(struct sol *src);

void init_GA(int size);
void done_GA();
int repeat_GA(double elapsed);

void crossover_GA(struct sol *gene);
void mutation_GA(struct sol *gene);
void replace_GA(struct sol **gene, int num_replace);

void pop_print(int n);
void gen_print();

#endif
