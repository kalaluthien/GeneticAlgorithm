#ifndef __GA_H_
#define __GA_H_

#include "graph.h"
#include "sol.h"

#define MUTATION_RATIO 0.02

void init_GA();
void done_GA();
int repeat_GA(double elapsed);
void step_GA();

void crossover_GA(struct sol *e, int *p);
void mutation_GA(struct sol *e);
void replace_GA(struct sol *e, int p);

void print_gen();

#endif
