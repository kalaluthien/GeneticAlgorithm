#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "timer.h"
#include "parser.h"
#include "graph.h"
#include "ga.h"

#define NUM_SOLUTIONS 100

static void initialize(int argc, char *argv[]);
static void run_ga();
static void print_solution();

/* global variables */
struct graph g;

/* local variables */
static struct timer *t_main;
static FILE *in;
static FILE *out;

int main(int argc, char *argv[]) {
  initialize(argc, argv);

  click_timer(t_main, 0);
  run_ga();
  click_timer(t_main, 0);

  print_solution();

  return 0;
}

static void initialize(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: ./%s [InputFile] [OutputFile]\n", argv[0]);
    throw("invalid arguments.");
  }

  parse_cmd(argv, &in, &out);
  parse_input(in, &g);

  t_main = create_timer();
}

static void run_ga() {
  struct sol offspring;

  init_GA(&g, NUM_SOLUTIONS);

  while (repeat_GA()) {
    crossover_GA(&offspring);

    mutation_GA(&offspring);

    replace_GA(&offspring);
  }
}

static void print_solution() {
  /* Dummy solution */
  printf("Time elapsed: %lf sec\n", read_timer(t_main, 0));
}
