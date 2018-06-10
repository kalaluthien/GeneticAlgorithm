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
  int u, sum = 0;
  char *rep = e->r;

  if (rep[0] == 0)
    norm_sol(e);

  for (u = 1; u <= len; u++) {
    if (rep[u-1] == 0) {
      for (struct node *n = g.l[u]->head; n; n = n->next) {
        if (n->v > len)
          break;

        sum += (rep[n->v-1] == 1) * n->w;
      }
    }
  }

  return e->v = sum;
}

int update_sol(struct sol *e, int u, int len) {
  int diff = 0;
  char *rep = e->r;

  int c = rep[u-1];
  for (struct node *n = g.l[u]->head; n; n = n->next) {
    if (n->v > len)
      break;

    int sign = ((c == rep[n->v-1]) << 1) - 1;
    diff += sign * n->w;
  }

  if (diff > 0) {
    rep[u-1] ^= 1;
    e->v += diff;
  }

  return diff;
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

struct sol *next_set(struct set *s, int v) {
  int min = 1, max = s->num, mid;

  while (min < max) {
    mid = (min + max) / 2;

    if (index_set(s, mid)->v >= v) {
      min = mid + 1;
    }
    else {
      max = mid;
    }
  }

  if (min > max) {
    return NULL;
  }
  else {
    return clone_sol(index_set(s, max));
  }
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

static void expand_set(struct set *s, int inc);

void merge_set(struct set *dst, struct set *src) {
  expand_set(dst, src->num);

  for (int i = 1; i <= src->num; i++) {
    insert_set(dst, src->e[i]);
  }

  sort_set(dst);
  fit_set(dst);
}

static void expand_set(struct set *s, int inc) {
  s->size += inc;
  s->e = (struct sol **) realloc(s->e, sizeof(struct sol *) * (s->size+1));
  s->idx = (int *) realloc(s->idx, sizeof(int) * (s->size+1));
  s->fit = (double *) realloc(s->fit, sizeof(double) * (s->size+1));
}

void lengthen_set(struct set *s, int len) {
  s->len = len;

  for (int i = 1; i <= s->num; i++) {
    eval_sol(s->e[i], s->len);
  }
}
/*
static void sort_set_internal(struct set *s, int p, int q) {
  int x = index_set(s, p)->v;
  int i = p-1;
  int j = q+1;

  while (i < j) {
    while (index_set(s, i)->v <= x) // FIXME: segfault
      i++;

    while (index_set(s, j)->v >= x)
      j--;

    if (i < j) {
      int t = s->idx[i];
      s->idx[i] = s->idx[j];
      s->idx[j] = t;
    }
  }

  sort_set_internal(s, p, j);
  sort_set_internal(s, j+1, q);
}
*/
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
  //sort_set_internal(s, 1, s->num);
}
