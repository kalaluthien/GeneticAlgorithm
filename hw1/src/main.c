#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "timer.h"
#include "parser.h"
#include "graph.h"
#include "ga.h"

static void initialize(int argc, char *argv[]);
static void run_ga();
static void print_result();

/* global variables */
struct graph g;

/* local variables */
static struct timer *t_main;
static FILE *in;
static FILE *out;

int main(int argc, char *argv[]) {
  initialize(argc, argv);

  t_main = create_timer();

  click_timer(t_main, 0);
  run_ga();
  click_timer(t_main, 0);

  print_result();

  return 0;
}

static void initialize(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: ./%s [InputFile] [OutputFile]\n", argv[0]);
    throw(1, "invalid arguments.");
  }

  parse_cmd(argv, &in, &out);
  parse_input(in, &g);
}

static void run_ga() {
  double decay;
  struct sol offspring[NUM_OFFSPRING];

  init_GA(NUM_SOLUTIONS);

  for (int i = 0; repeat_GA(read_timer(t_main, 1)); i++) {
#ifdef PRINT_GENERATION
    if (i % 10 == 0) {
      gen_print(i);
    }
#endif

    click_timer(t_main, 1);
    {
      decay = read_timer(t_main, 1) / MAX_TIME;

      for (int i = 0; i < NUM_OFFSPRING; i++) {
        crossover_GA(&offspring[i], decay);
        mutation_GA(&offspring[i], decay);
      }

      replace_GA(offspring, NUM_OFFSPRING);
    }
    click_timer(t_main, 1);
  }
}

static void print_result() {
  double total_elapsed = read_timer(t_main, 0);
  double ga_elapsed = read_timer(t_main, 1);

  pop_print();
  printf(" GA total: %lf sec\n", ga_elapsed);
  printf(" Overhead: %lf sec\n\n", total_elapsed - ga_elapsed);

  unsigned char *result = best_solution();

  for (int i = 0; i < g.num_vtx; i++) {
    if (result[i]) {
      fprintf(out, "%d ", i+1);
    }
  }
}
