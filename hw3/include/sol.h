#ifndef __SOL_H__
#define __SOL_H__

#include "common.h"
#include "graph.h"

////////////////////////////////////////////////////////////
// Solution
////////////////////////////////////////////////////////////

struct sol {
  char r[MAX_VTX];
  int v;
};

struct sol *new_sol();
struct sol *clone_sol(struct sol *e);

void zero_sol(struct sol *e);
void rand_sol(struct sol *e);
void norm_sol(struct sol *e);
int eval_sol(struct sol *e, int len);
void flip_sol(struct sol *e, int u, int diff);
void print_sol(struct sol *e);

////////////////////////////////////////////////////////////
// Set
////////////////////////////////////////////////////////////

#define FIT_MAX (1.50 / NUM_SOLUTION)
#define FIT_MIN (2.00 / NUM_SOLUTION - FIT_MAX)

struct set {
  struct sol **e;
  int *idx;
  double *fit;

  int size;
  int num;
  int len;
};

struct set *create_set(int size, int len);
void init_set(struct set *s);
void delete_set(struct set *s);

struct sol *best_set(struct set *s);
struct sol *next_set(struct set *s, int v);

#define index_set(s, i) ((s)->e[(s)->idx[i]])
#define quart_set(s, q) ((s)->e[(s)->idx[(q)*s->num/QNUM+1]])

void insert_set(struct set *s, struct sol *e);
void replace_set(struct set *s, struct sol *e, int i);
void merge_set(struct set *dst, struct set *src);
void lengthen_set(struct set *s, int len);
void sort_set(struct set *s);

#endif
