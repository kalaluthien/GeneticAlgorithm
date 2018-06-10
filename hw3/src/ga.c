#include <limits.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "sol.h"
#include "ga.h"

/* global environment */
extern struct graph g;
extern struct set *s;
extern struct set *s_inc;
extern struct set *s_sav;

static int gen;
static int order[MAX_VTX+1];

/* GA operation */
void init_GA() {
  srand(time(NULL));

  int size = NUM_SOLUTION;
  int len = g.num_vtx;

  s = create_set(size, len);
  init_set(s);

  for (int i = 0; i <= len; i++) {
    order[i] = i;
  }

  gen = 0;
}

void done_GA() {
  delete_set(s);
  s = NULL;
}

static int converged(int n, int m);

int repeat_GA(double elapsed) {
  return (elapsed < TIME_PER_PHASE) && !converged(0, QNUM-1);
}

void step_GA() {
}

static int select_GA();

void crossover_GA(struct sol *e, int *p) {
  int p1 = select_GA();
  int p2 = select_GA();

  char *r0 = e->r;
  char *r1 = index_set(s, p1)->r;
  char *r2 = index_set(s, p2)->r;

  int q = s->len / 8;
  int r = s->len % 8;

  for (int i = 0; i < q; i++) {
    int rv = rand();
    r0[8*i+0] = ((rv >> 0) & 1) ? r1[8*i+0] : r2[8*i+0];
    r0[8*i+1] = ((rv >> 1) & 1) ? r1[8*i+1] : r2[8*i+1];
    r0[8*i+2] = ((rv >> 2) & 1) ? r1[8*i+2] : r2[8*i+2];
    r0[8*i+3] = ((rv >> 3) & 1) ? r1[8*i+3] : r2[8*i+3];
    r0[8*i+4] = ((rv >> 4) & 1) ? r1[8*i+4] : r2[8*i+4];
    r0[8*i+5] = ((rv >> 5) & 1) ? r1[8*i+5] : r2[8*i+5];
    r0[8*i+6] = ((rv >> 6) & 1) ? r1[8*i+6] : r2[8*i+6];
    r0[8*i+7] = ((rv >> 7) & 1) ? r1[8*i+7] : r2[8*i+7];
  }
  for (int i = 0; i < r; i++) {
    r0[8*q+i] = (rand() & 1) ? r1[8*q+i] : r2[8*q+i];
  }

  *p = (index_set(s, p1)->v < index_set(s, p2)->v) ? p2 : p1;
}

static int select_GA() {
  double fit = (double) rand() / RAND_MAX;
  int min = 1, max = s->num, mid;

  while (min < max) {
    mid = (min + max) / 2;

    if (s->fit[mid] < fit) {
      min = mid + 1;
    }
    else {
      max = mid;
    }
  }

  return max;
}

void mutation_GA(struct sol *e) {
  int count = s->len * MUTATION_RATIO + 1;

  while (count-- > 0) {
    e->r[rand() % s->len] ^= 1;
  }
}

static void bit_flip(struct sol *e);

void replace_GA(struct sol *e, int p) {
  eval_sol(e, s->len);
  bit_flip(e);

  if (p > 0) {
    struct sol *o = index_set(s, p);

    if (e->v > o->v) {
      replace_set(s, e, p);
    }
  }
  else {
    replace_set(s, e, s->num);
  }

  sort_set(s);

  gen++;
}

/* helper functions */
static void shuffle_order() {
  for (int i = 1; i < s->len; i++) {
    int j = rand() % (s->len - i + 1) + i;
    int tmp = order[i];
    order[i] = order[j];
    order[j] = tmp;
  }
}

static void bit_flip(struct sol *e) {
  int improved;

  shuffle_order();

  do {
    improved = 0;
    for (int i = 1; i <= s->len; i++) {
      if (update_sol(e, order[i], s->len) > 0) {
        improved = 1;
      }
    }
  } while (improved);
}

static int converged(int n, int m) {
  int v_n = quart_set(s, n)->v;
  int v_m = quart_set(s, m)->v;
  return (v_n == v_m);
}

void print_gen() {
  char info[1000];
  int n = 0;

  for (int i = 0; i < QNUM; i++) {
    int qv = quart_set(s, i)->v;
    n += sprintf(info + n, " %7d", qv);
  }

  printf("\r[%6d]%s", gen, info);
  fflush(stdout);
}
