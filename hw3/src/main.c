#include <limits.h>

#include "common.h"
#include "timer.h"
#include "graph.h"
#include "sol.h"
#include "ga.h"

/* global environment */
struct graph g;
struct set *s;
struct set *s_sav;
double time_remain;

static struct timer *t_main;

static FILE *in;
static FILE *out;
static char *in_fname = "maxcut.in";
static char *out_fname = "maxcut.out";

static void init_phase();
static void run_phase();
static void done_phase();
static void print_stats();

int main(int argc, char *argv[]) {
  t_main = create_timer();

  click_timer(t_main, 0);

  if (argc != 1 && argc != 3) {
    printf("Usage: ./%s ([InputFile] [OutputFile])\n", argv[0]);
    throw(1, "invalid arguments.");
  }
  else if (argc == 3) {
    in_fname = argv[1];
    out_fname = argv[2];
  }

  in = fopen(in_fname, "r");
  throw(in == NULL, "faild to open input.");

  time_remain = TIME_PER_PHASE * NUM_PHASE;

  create_graph(&g, in);
  s_sav = create_set(NUM_PHASE, g.num_vtx);

  for (int p = 0; p < NUM_PHASE; p++) {
    init_phase();

    run_phase();

    done_phase();
  }

  click_timer(t_main, 0);

  print_stats();

  return 0;
}

static void init_phase() {
  clear_timer(t_main, 1);

  init_GA();
}

static void run_phase() {
  int p1, p2;
  struct sol *offspring;

  for (int i = 0; repeat_GA(read_timer(t_main, 1)); i++) {
    click_timer(t_main, 1);

    print_gen();

    offspring = new_sol();

    crossover_GA(offspring, &p1, &p2);

    mutation_GA(offspring);

    replace_GA(offspring, p1, p2);

    click_timer(t_main, 1);
  }
}

static void save_file(struct sol *e);

static void done_phase() {
  struct sol *result = best_set(s);
  throw(result == NULL, "faild to get best solution.");

  time_remain -= read_timer(t_main, 1);

  insert_set(s_sav, clone_sol(result));

  sort_set(s_sav);

  save_file(best_set(s_sav));

  done_GA();
}

static void save_file(struct sol *e) {
  out = fopen(out_fname, "w");
  throw(out == NULL, "faild to open output.");

  char *rep = e->r;

  for (int u = 1; u <= g.num_vtx; u++) {
    if (rep[u-1] == 1) {
      fprintf(out, "%d", u);

      if (u != g.num_vtx)
        fprintf(out, " ");
    }
  }

  fclose(out);
}

static void print_stats() {
  double total_elapsed = read_timer(t_main, 0);
  printf("\n\n best = %d\n", best_set(s_sav)->v);
  printf(" elapsed = %lf sec\n\n", total_elapsed);
}
