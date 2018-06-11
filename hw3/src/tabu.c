#include "tabu.h"

/* global environment */
extern struct graph g;
extern struct set *s;

/* local environment */
static int improved[2];
static int order[MAX_VTX+1];

static void init_order() {
  for (int i = 1; i <= s->len; i++) {
    order[i] = i;
  }
}

static void shuffle_order() {
  for (int i = 1; i < s->len; i++) {
    int j = rand() % (s->len - i + 1) + i;
    int tmp = order[i];
    order[i] = order[j];
    order[j] = tmp;
  }
}

static int gain(struct sol *e, int u) {
  int *vp = g.v[u];
  int *wp = g.w[u];
  int size = g.size[u];

  char *rep = e->r;
  char c = rep[u-1];

  int gain = 0;

  for (int i = 0; i < size; i++) {
    int v = vp[i];

    if (v > s->len)
      continue;

    gain += ((c == rep[v-1]) * 2 - 1) * wp[i];
  }

  return gain;
}

static void one_bit_flip(struct sol *e) {
  for (int i = 1; i <= s->len; i++) {
    int gi = gain(e, order[i]);

    if (gi > 0) {
      flip_sol(e, order[i], gi);
      improved[0] = 1;
    }
  }
}

static void two_bit_flip(struct sol *e) {
  char *rep = e->r;

  for (int i = 1; i <= s->len; i++) {
    int u = order[i];
    int *vp = g.v[u];
    int *wp = g.w[u];

    for (int j = 0; j < g.size[u]; j++) {
      int v = vp[j];

      if (v < u || v > s->len)
        continue;

      int gu = gain(e, u);
      int gv = gain(e, v);
      int gw = ((rep[u-1] != rep[v-1]) * 2 - 1) * wp[j];

      if (gu + gv + 2 * gw > 0) {
        flip_sol(e, u, gu + gw);
        flip_sol(e, v, gv + gw);
        improved[1] = 1;
      }
    }
  }
}

void tabu_search(struct sol *e) {
  init_order();
  shuffle_order();

  do {
    improved[0] = 0;
    improved[1] = 0;

    one_bit_flip(e);
    two_bit_flip(e);

  } while (improved[0] || improved[1]);
}
