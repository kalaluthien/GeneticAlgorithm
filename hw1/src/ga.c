#include <stdlib.h>

#include "ga.h"

static struct {
  struct sol *sols;
  int size;
  int len;
} population;

static void randomize_sols();

void init_GA(struct graph *g, int num_sols) {
  population.sols = (struct sol*) malloc(num_sols * sizeof(struct sol));
  population.size = num_sols;
  population.len = g->num_vtx;

  randomize_sols();
}

static void randomize_sols() {
}

int repeat_GA() {
  return 0;
}

void crossover_GA(struct sol *offspring) {
}

void mutation_GA(struct sol *offspring) {
}

void replace_GA(struct sol *offspring) {
}
