#include <string.h>
#include "sol.h"

extern struct graph g;

////////////////////////////////////////////////////////////
// Solution Operations
////////////////////////////////////////////////////////////

struct sol *new_sol() {
  return (struct sol *) malloc(sizeof(struct sol));
}

struct sol *clone_sol(struct sol *src) {
  if (src == NULL)
    return NULL;

  struct sol *dst = new_sol();

  if (dst != NULL)
    memcpy(dst, src, sizeof(struct sol));

  return dst;
}

void zero_sol(struct sol *e) {
  memset(e->r, 0, sizeof(e->r));
}

void rand_sol(struct sol *e) {
  char *rep = e->r;

  int q = sizeof(e->r) / 8;
  int r = sizeof(e->r) % 8;

  for (int i = 0; i < q; i++) {
    int rv = rand();
    rep[8*i+0] = (rv >> 0) & 1;
    rep[8*i+1] = (rv >> 1) & 1;
    rep[8*i+2] = (rv >> 2) & 1;
    rep[8*i+3] = (rv >> 3) & 1;
    rep[8*i+4] = (rv >> 4) & 1;
    rep[8*i+5] = (rv >> 5) & 1;
    rep[8*i+6] = (rv >> 6) & 1;
    rep[8*i+7] = (rv >> 7) & 1;
  }
  for (int i = 0; i < r; i++) {
    rep[8*q+i] = rand() & 1;
  }

  rep[1] = 1;
}

void norm_sol(struct sol *e) {
  char *rep = e->r;

  int q = sizeof(e->r) / 8;
  int r = sizeof(e->r) % 8;

  for (int i = 0; i < q; i++) {
    rep[8*i+0] ^= 1;
    rep[8*i+1] ^= 1;
    rep[8*i+2] ^= 1;
    rep[8*i+3] ^= 1;
    rep[8*i+4] ^= 1;
    rep[8*i+5] ^= 1;
    rep[8*i+6] ^= 1;
    rep[8*i+7] ^= 1;
  }
  for (int i = 0; i < r; i++) {
    rep[8*q+i] ^= 1;
  }
}

int eval_sol(struct sol *e, int len) {
  char *rep = e->r;

  if (rep[0] == 0)
    norm_sol(e);

  int sum = 0;

  for (int u = 1; u <= len; u++) {
    int *vp = g.v[u];
    int *wp = g.w[u];
    int size = g.size[u];

    if (rep[u-1] == 0) {
      for (int i = 0; i <size; i++) {
        int v = vp[i];

        if (v > len)
          continue;

        sum += (rep[v-1] == 1) * wp[i];
      }
    }
  }

  return e->v = sum;
}

void flip_sol(struct sol *e, int u, int diff) {
  e->r[u-1] ^= 1;
  e->v += diff;
}

void print_sol(struct sol *e) {
  printf("%4d | ", e->v);
  for (int i = 0; i < sizeof(e->r); i++) {
    printf("%d", e->r[i]);
  }
  printf("\n");
}

////////////////////////////////////////////////////////////
// Set Operations
////////////////////////////////////////////////////////////

static void fit_set(struct set *s) {
  double fit_val = FIT_MAX;
  double fit_del = (FIT_MAX - FIT_MIN) / (s->num - 1);

  for (int i = 1; i <= s->num; i++) {
    s->fit[i] = s->fit[i-1] + fit_val;
    fit_val -= fit_del;
  }
}

struct set *create_set(int size, int len) {
  size = (size < 1) ? 1 : size;
  len = (len < 1) ? 1 : len;

  struct set *s = (struct set *) malloc(sizeof(struct set));

  s->e = (struct sol **) malloc(sizeof(struct sol *) * (size+1));
  s->idx = (int *) malloc(sizeof(int) * (size+1));
  s->fit = (double *) malloc(sizeof(double) * (size+1));
  s->size = size;
  s->num = 0;
  s->len = len;

  return s;
}

void init_set(struct set *s) {
  struct sol *e = NULL;

  for (int i = 1; i <= s->size; i++) {
    e = new_sol();

    rand_sol(e);
    eval_sol(e, s->len);

    insert_set(s, e);
  }

  sort_set(s);
  fit_set(s);
}

void delete_set(struct set *s) {
  throw(s == NULL, "faild to delete solution set.");

  for (int i = 1; i <= s->num; i++) {
    free(s->e[i]);
  }
  free(s->e);
  free(s->idx);
  free(s->fit);
  free(s);
}

struct sol *best_set(struct set *s) {
  return index_set(s, 1);
}

void insert_set(struct set *s, struct sol *e) {
  if (s->num < s->size) {
    s->num++;
    s->e[s->num] = e;
    s->idx[s->num] = s->num;
  }
}

void replace_set(struct set *s, struct sol *e, int i) {
  struct sol *o = index_set(s, i);
  index_set(s, i) = e;
  free(o);
}

void sort_set(struct set *s) {
  for (int i = s->num; i > 1; i--) {
    for (int j = 1; j < i; j++) {
      if (index_set(s, j)->v < index_set(s, j+1)->v) {
        int tmp = s->idx[j];
        s->idx[j] = s->idx[j+1];
        s->idx[j+1] = tmp;
      }
    }
  }
}
