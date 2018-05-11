#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "ga.h"

static int compare_sol(const void *a, const void *b);
static void normalize_sol(struct sol *gene);
static void bit_flip(struct sol *gene);
static int select_GA();
static int converged(int n, int m);
static void sol_print(struct sol *sol, int i);

/* global variable */
extern struct graph g;
extern int profiled;
extern int reset_count;
extern struct sol *save_list[NUM_SURVIVORS+1];
extern int save_size;

/* local variable */
static int order[MAX_VTX+1];

static struct {
  int num;
  int len;
  int gen;

  struct sol *item[NUM_SOLUTIONS+1];
  int idx[NUM_SOLUTIONS+1];

  double fits[NUM_SOLUTIONS+1];
} p;

/* solution management */
static void sort_sol() {
  qsort(p.idx+1, p.num, sizeof(int), compare_sol);
}

static void replace_sol(struct sol *gene, int i) {
  struct sol *old = p.item[p.idx[i]];
  p.item[p.idx[i]] = gene;
  free(old);
}

static void clear_sol() {
  for (int i = 1; i <= p.num; i++) {
    free(p.item[i]);
  }
}

static void zero_sol(struct sol *gene) {
  memset(gene->rep, 0, sizeof(gene->rep));
}

static void randomize_sol(struct sol *gene) {
  int *rep = gene->rep;

  int q = p.len / 8;
  int r = p.len % 8;

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

static void eval_sol(struct sol *gene) {
  int u, sum = 0;
  int *r = gene->rep;

  if (r[0] == 0) {
    normalize_sol(gene);
  }

  for (u = 1; u <= p.len; u++) {
    if (r[u-1] == 0) {
      for (struct node *n = g.l[u]->head; n; n = n->next) {
        sum += (r[n->v-1] == 1) * n->w;
      }
    }
  }

  gene->val = sum;
}

static int update_sol(struct sol *gene, int u) {
  int diff = 0;
  int *r = gene->rep;

  int c = r[u-1];
  for (struct node *n = g.l[u]->head; n; n = n->next) {
    int sign = (c == r[n->v-1]) ? 1 : -1;
    diff += sign * n->w;
  }

  if (diff > 0) {
    r[u-1] ^= 1;
    gene->val += diff;
  }

  return diff;
}

static void normalize_sol(struct sol *gene) {
  int *rep = gene->rep;

  int q = p.len / 8;
  int r = p.len % 8;

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

static int compare_sol(const void *a, const void *b) {
  int x = p.item[*((int *) a)]->val;
  int y = p.item[*((int *) b)]->val;
  return (x < y) - (x > y);
}

struct sol *new_sol() {
  return (struct sol *) malloc(sizeof(struct sol));
}

struct sol *best_sol() {
  return p.item[p.idx[1]];
}

struct sol *next_sol(int val) {
  int min = 1, max = p.num, mid;

  while (min < max) {
    mid = (min + max) / 2;

    if (p.item[p.idx[mid]]->val >= val) {
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
    return clone_sol(p.item[p.idx[max]]);
  }
}

struct sol *clone_sol(struct sol *src) {
  struct sol *dst = NULL;

  if (src != NULL && (dst = new_sol()) != NULL) {
    memcpy(dst, src, sizeof(struct sol));
  }

  return dst;
}

void copy_sol(struct sol *dst, struct sol *src) {
  memcpy(dst->rep, src->rep, sizeof(int) * p.len);
}

/* GA operation */
void init_GA(int pop_size) {
  struct sol *new_gene;

  memset(p.item, 0, sizeof(p.item));
  p.num = pop_size;
  p.len = g.num_vtx;
  p.gen = 0;

  srand(time(NULL));

  for (int i = 1; i <= p.num; i++) {
    new_gene = new_sol();

    if (profiled && (i <= save_size) && save_list[i-1]) {
      copy_sol(new_gene, save_list[i-1]);
      eval_sol(new_gene);
    }
    else if (i >= 7 * p.num / 8) {
      zero_sol(new_gene);
      eval_sol(new_gene);
    }
    else {
      randomize_sol(new_gene);
      eval_sol(new_gene);
    }

    p.item[i] = new_gene;
    p.idx[i] = i;
  }

  sort_sol();

  double fitness = profiled ? FIT_MAX_REAL : FIT_MAX_PROF;
  int fit_diff = profiled
               ? (FIT_MAX_REAL - FIT_MIN_REAL) / (p.num - 1)
               : (FIT_MAX_PROF - FIT_MIN_PROF) / (p.num - 1);

  for (int i = 1; i <= p.num; i++) {
    p.fits[i] = p.fits[i-1] + fitness;
    fitness -= fit_diff;
  }

  for (int i = 0; i <= p.len; i++) {
    order[i] = i;
  }
}

void done_GA() {
  clear_sol();
}

int repeat_GA(double elapsed) {
  int repeat;

  if (!profiled) {
    repeat =  (elapsed < UNIT_TIME * (reset_count + 1))
           && (reset_count < PROF_COUNT)
           && !converged(0, 7);
  }
  else {
    repeat =  (elapsed < TOTAL_TIME)
           && (elapsed < UNIT_TIME * (reset_count + 1))
           && !converged(0, 7);
  }

  p.gen++;

  if (!repeat) {
    reset_count++;
    profiled = (reset_count >= PROF_COUNT);
  }

  return repeat;
}

void crossover_GA(struct sol *gene) {
  int p1 = select_GA();
  int p2 = select_GA();

  int *r0 = gene->rep;
  int *r1 = p.item[p.idx[p1]]->rep;
  int *r2 = p.item[p.idx[p2]]->rep;

  int q = p.len / 8;
  int r = p.len % 8;

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
}

static int select_GA() {
  double fit = (double) rand() / RAND_MAX;
  int min = 1, max = p.num, mid;

  while (min < max) {
    mid = (min + max) / 2;

    if (p.fits[mid] < fit) {
      min = mid + 1;
    }
    else {
      max = mid;
    }
  }

  return max;
}

void mutation_GA(struct sol *gene) {
  int count = p.len * MUTATION_RATIO;
  if (count < 2) count = 2;

  while (count-- > 0) {
    gene->rep[rand() % p.len] ^= 1;
  }
}

void replace_GA(struct sol **gene, int num_replace) {
  int replace_list[NUM_SOLUTIONS+1];
  int drop_list[NUM_OFFSPRING];

  for (int i = 1; i <= p.num; i++) {
    replace_list[i] = -1;
  }

  for (int j = 0; j < num_replace; j++) {
    eval_sol(gene[j]);
#ifdef LOCAL_OPTIMIZE
    bit_flip(gene[j]);
#endif
    drop_list[j] = 1;

    int val = gene[j]->val;

    for (int i = 1; i <= p.num; i++) {
      if (replace_list[i] == -1) {
        if (val > p.item[p.idx[i]]->val) {
          drop_list[j] = 0;
          replace_list[i] = j;
          break;
        }
      }
      else {
        if (val > gene[replace_list[i]]->val) {
          drop_list[replace_list[i]] = 1;
          drop_list[j] = 0;
          replace_list[i] = j;
          break;
        }
      }
    }
  }

  for (int i = 1; i <= p.num; i++) {
    if (replace_list[i] != -1) {
      replace_sol(gene[replace_list[i]], i);
    }
  }

  for (int j = 0; j < num_replace; j++) {
    if (drop_list[j] != 0) {
      free(gene[j]);
    }
  }

  sort_sol();
}

/* local optimization */
static void shuffle_order() {
  for (int i = 1; i < p.len; i++) {
    int j = rand() % (p.len - i + 1) + i;
    int tmp = order[i];
    order[i] = order[j];
    order[j] = tmp;
  }
}

static void bit_flip(struct sol *gene) {
  int improved;

  shuffle_order();

  do {
    improved = 0;
    for (int i = 1; i <= p.len; i++) {
      if (update_sol(gene, order[i]) > 0) {
        improved = 1;
      }
    }
  } while (improved);
}

/* helper functions */
static int converged(int n, int m) {
  int v_n = p.item[p.idx[n*p.num/QNUM+1]]->val;
  int v_m = p.item[p.idx[m*p.num/QNUM+1]]->val;
  return (v_n == v_m);
}

static void sol_print(struct sol *sol, int i) {
  printf(" [%3d] %4d ", i, sol->val);
  for (int j = 0; j < p.len; j++) {
    printf("%d", sol->rep[j]);
  }
  printf("\n");
}

void pop_print(int n) {
  printf("\n====== population stats ======\n\n");
  for (int i = 1; i <= p.num && i <= n; i++) {
    sol_print(p.item[p.idx[i]], i);
  }
  printf("\n");
}

void gen_print() {
  int values[NUM_SOLUTIONS];
  char info[1000];
  int n = 0;

  int Q0 = p.item[p.idx[0*p.num/QNUM+1]]->val;
  int Q1 = p.item[p.idx[2*p.num/QNUM+1]]->val;
  int Q2 = p.item[p.idx[4*p.num/QNUM+1]]->val;
  int Q3 = p.item[p.idx[6*p.num/QNUM+1]]->val;

  for (int i = 1; i <= p.num; i++) {
    values[i-1] = p.item[p.idx[i]]->val;
  }
  
  for (int i = 0; i < QNUM; i++) {
    int qval = p.item[p.idx[i*p.num/QNUM+1]]->val;
    n += sprintf(info + n, " %4d", qval);
  }

  printf("\r[%d %d]%+6d\t%s", profiled, reset_count, p.gen, info);
  fflush(stdout);
}
