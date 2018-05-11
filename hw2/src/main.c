#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "timer.h"
#include "parser.h"
#include "graph.h"
#include "ga.h"

static void initialize(int argc, char *argv[]);
static void run_ga();
static void save_result();
static void print_stats();

/* global variables */
struct graph g;
int profiled;
int reset_count;
struct sol *save_list[NUM_SURVIVORS+1];
int save_size;

/* local variables */
static struct sol *best_result;
static struct timer *t_main;
static FILE *in;
static FILE *out;
static char *in_fname = "maxcut.in";
static char *out_fname = "maxcut.out";

int main(int argc, char *argv[]) {
  initialize(argc, argv);

  t_main = create_timer();

  click_timer(t_main, 0);
  run_ga();
  click_timer(t_main, 0);

  print_stats();

  return 0;
}

static void initialize(int argc, char *argv[]) {
  if (argc != 1 && argc != 3) {
    printf("Usage: ./%s [InputFile] [OutputFile]\n", argv[0]);
    throw(1, "invalid arguments.");
  }

  if (argc == 3) {
    in_fname = argv[1];
    out_fname = argv[2];
  }

  throw(!(in = fopen(in_fname, "r")),
        "faild to open input.\n");

  parse_input(in, &g);
}

static void run_ga() {
  struct sol *offspring[NUM_OFFSPRING];

  while (1) {
    int pop_size = profiled ? NUM_SOLUTIONS : NUM_PROFILING;
    init_GA(pop_size);

    for (int i = 0; repeat_GA(read_timer(t_main, 1)); i++) {
      if (i % 10 == 0) {
        gen_print();
        //pop_print(16);
      }

      click_timer(t_main, 1);

      for (int j = 0; j < NUM_OFFSPRING; j++) {
        offspring[j] = new_sol();
        crossover_GA(offspring[j]);
        mutation_GA(offspring[j]);
      }
      replace_GA(offspring, NUM_OFFSPRING);

      click_timer(t_main, 1);
    }

    save_result();
  }
}

static void save_result() {
  struct sol *gene = best_sol();

  /* save local optimal */
  if (reset_count < PROF_COUNT) {
    int uniq = 1;

    for (int i = 0; i < save_size; i++) {
      uniq &= gene->val != save_list[i]->val;
    }

    if (uniq) {
      save_list[save_size++] = clone_sol(gene);
    }
  }
  else {
    save_list[0] = gene;
    save_size = 1;

    while (save_size < NUM_SURVIVORS) {
      save_list[save_size] = next_sol(save_list[save_size]);

      if (save_list[save_size] == NULL)
        break;

      save_size++;
    }
  }
    /*
    int min = 0;
    int max = 0;

    for (int i = 1; i < save_size; i++) {
      int val = save_list[i]->val;
      min = (val < save_list[min]->val) ? i : min;
      max = (val > save_list[max]->val) ? i : max;
    }

    if (save_list[max]->val < gene->val) {
      free(save_list[min]);
      save_list[min] = clone_sol(gene);
    }
    */

  /* save best solution */
  if (best_result == NULL) {
    best_result = clone_sol(gene);
  }
  else if (best_result->val < best_sol()->val) {
    free(best_result);
    best_result = clone_sol(gene);
  }

  /* dump best solution */
  throw(!(out = fopen(out_fname, "w")),
        "faild to open output.\n");

  for (int i = 0; i < g.num_vtx; i++) {
    if (best_result->rep[i]) {
      fprintf(out, "%d", i+1);
      if (i != g.num_vtx-1) {
        fprintf(out, " ");
      }
    }
  }

  fclose(out);

  done_GA();
}

static void print_stats() {
  double total_elapsed = read_timer(t_main, 0);
  double ga_elapsed = read_timer(t_main, 1);

  printf("\n\n best = %d\n", best_result->val);
  printf(" elapsed = %lf sec (%lf sec)\n\n",
         total_elapsed, ga_elapsed);
}
