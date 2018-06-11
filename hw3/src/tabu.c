#include "tabu.h"

/* global environment */
extern struct graph g;
extern struct set *s;

/* local environment */
static int improved;
static int order[MAX_VTX+1];
//static int delta[MAX_VTX+1];

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
  int gain = 0;
  char *rep = e->r;
  char c = rep[u-1];

  for (struct node *n = g.l[u]->head; n; n = n->next) {
    int v = n->v;

    if (v > s->len)
      break;

    int sign = (c == rep[v-1]) * 2 - 1;
    gain += sign * n->w;
  }

  return gain;
}

static void bit_flip_order(struct sol *e) {
  for (int i = 1; i <= s->len; i++) {
    int gi = gain(e, order[i]);

    if (gi > 0) {
      flip_sol(e, order[i], gi);
      improved = 1;
    }
  }
}

static void bit_swap_order(struct sol *e) {
  char *rep = e->r;

  for (int i = 1; i < s->len; i++) {
    for (struct node *n = g.l[order[i]]->head; n; n = n->next) {
      int j = n->v;

      if (j > s->len)
        break;

      int gi = gain(e, order[i]);
      int gj = gain(e, j);

      int sign = (rep[order[i]-1] != rep[j-1]) * 4 - 2;
      int gd = sign * n->w;

      if (gi + gj + gd > 0) {
        flip_sol(e, order[i], gi);
        flip_sol(e, j, gj + gd);

        improved = 1;
      }
    }
  }
}
/*
static void init_delta(struct sol *e) {
  int gain;
  char *rep = e->r;

  for (int u = 1; u <= s->len; u++) {
    int c = rep[u-1];
    gain = 0;

    for (struct node *n = g.l[u]->head; n; n = n->next) {
      int v = n->v;

      if (v > s->len)
        break;

      int sign = (c == rep[v-1]) * 2 - 1;
      gain += sign * n->w;
    }

    delta[u] = gain;
  }
}

static void update_delta(struct sol *e, int u) {
  char *rep = e->r;
  char c = rep[u-1];

  for (struct node *n = g.l[u]->head; n; n = n->next) {
    int v = n->v;

    if (v > s->len) {
      break;
    }
    else if (v == u) {
      delta[v] = -delta[v];
    }
    else {
      int sign = (c != rep[v-1]) * 4 - 2;
      delta[v] += sign * n->w;
    }
  }
}

static void bit_flip_best(struct sol *e) {
  int u = 1;

  init_delta(e);

  for (int i = 2; i <= s->len; i++) {
    u = (delta[u] < delta[i]) ? i : u;
  }

  if (delta[u] > 0) {
    flip_sol(e, u, delta[u]);

    update_delta(e, u);

    improved = 1;
  }
}
*/

void tabu_search(struct sol *e) {
  init_order();
  shuffle_order();

  do {
    improved = 0;

    bit_flip_order(e);
    bit_swap_order(e);

  } while (improved);
}
