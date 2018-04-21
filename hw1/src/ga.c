#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "ga.h"

#define STATS_ENABLED 1
#define DETAILS_ENABLED 0

static int distance_GA(unsigned char *r1, unsigned char *r2);
static void select_GA(int *i);

/* nonlocal variable */
extern struct graph g;

/* population management */
static struct {
  struct sol item[NUM_SOLUTIONS+1];
  int idx[NUM_SOLUTIONS+1];
  int num;
  int len;

  double fit_cum[NUM_SOLUTIONS+1];

  int gen;

  double sum_of_val;
  double sum_of_val_square;
} pop_inst;

static void random_sol(struct sol *gene) {
  for (int i = 0; i < pop_inst.len / 4; i += 4) {
    gene->rep[4*i+0] = ((double) rand() / RAND_MAX < SEED_RATIO);
    gene->rep[4*i+1] = ((double) rand() / RAND_MAX < SEED_RATIO);
    gene->rep[4*i+2] = ((double) rand() / RAND_MAX < SEED_RATIO);
    gene->rep[4*i+3] = ((double) rand() / RAND_MAX < SEED_RATIO);
  }
  for (int i = 0; i < pop_inst.len % 4; i++) {
    gene->rep[i] = ((double) rand() / RAND_MAX < SEED_RATIO);
  }
}

static void validate_sol(struct sol *gene) {
  int sum = 0;

  for (int u = 0; u < g.num_vtx; u++) {
    for (int v = u+1; v < g.num_vtx; v++) {
      if (gene->rep[u] != gene->rep[v]) {
        sum += g.e[u][v];
      }
    }
  }

  gene->val = sum;
}

static int compare_sol(const void *a, const void *b) {
  int x = pop_inst.item[*((int *) a)].val;
  int y = pop_inst.item[*((int *) b)].val;

  return (x < y) - (x > y);
}

static void pop_sort() {
  qsort(pop_inst.idx+1, pop_inst.num, sizeof(int), compare_sol);
}

static void pop_replace(struct sol *gene, int i) {
  struct sol *old_gene = &pop_inst.item[pop_inst.idx[i]];
  
  if (STATS_ENABLED) {
    pop_inst.sum_of_val -= old_gene->val;
    pop_inst.sum_of_val += gene->val;
    pop_inst.sum_of_val_square -= old_gene->val * old_gene->val;
    pop_inst.sum_of_val_square += gene->val * gene->val;
  }

  *old_gene = *gene;
}

unsigned char *best_solution() {
  return pop_inst.item[pop_inst.idx[1]].rep;
}

/* GA operation */
void init_GA(int size) {
  static struct sol new_gene;

  memset(pop_inst.item, 0, sizeof(pop_inst.item));
  pop_inst.num = 0;
  pop_inst.len = g.num_vtx;
  pop_inst.gen = 0;

  srand(time(NULL));

  for (int i = 1; i <= size; i++) {
    pop_inst.idx[i] = i;
    pop_inst.num = i;

    random_sol(&new_gene);
    validate_sol(&new_gene);
    pop_replace(&new_gene, i);
  }

  pop_sort();

  double fitness = FIT_MAX;
  for (int i = 1; i <= pop_inst.num; i++) {
    pop_inst.fit_cum[i] = pop_inst.fit_cum[i-1] + fitness;
    fitness -= (FIT_MAX - FIT_MIN) / (pop_inst.num - 1);
  }
}

int repeat_GA(double elapsed) {
  int dist;
  unsigned char *r1, *r2;

  r1 = pop_inst.item[pop_inst.idx[1]].rep;

  for (int i = 2; i <= pop_inst.num; i++) {
    r2 = pop_inst.item[pop_inst.idx[i]].rep;
    dist = distance_GA(r1, r2);

    if (dist > MAX_VTX) {
      for (int j = 0; j < pop_inst.len / 4; j += 4) {
        r2[4*j+0] ^= 1;
        r2[4*j+1] ^= 1;
        r2[4*j+2] ^= 1;
        r2[4*j+3] ^= 1;
      }
      for (int j = 0; j < pop_inst.len % 4; j++) {
        r2[j] ^= 1;
      }
    }
  }

  pop_inst.gen++;

  return (elapsed < MAX_TIME);
}

static int distance_GA(unsigned char *r1, unsigned char *r2) {
  int dist = 0;
  int dist_sub[4] = { 0, 0, 0, 0 };

  for (int i = 0; i < pop_inst.len / 4; i += 4) {
    dist_sub[0] += (r1[4*i+0] != r2[4*i+0]);
    dist_sub[1] += (r1[4*i+1] != r2[4*i+1]);
    dist_sub[2] += (r1[4*i+2] != r2[4*i+2]);
    dist_sub[3] += (r1[4*i+3] != r2[4*i+3]);
  }
  for (int i = 0; i < pop_inst.len % 4; i++) {
    dist_sub[i] += (r1[i] != r2[i]);
  }

  dist = dist_sub[0] + dist_sub[1] + dist_sub[2] + dist_sub[3];

  return dist;
}

void crossover_GA(struct sol *gene, double decay) {
  int p1, p2;

  /* select 2 */
  select_GA(&p1);
  select_GA(&p2);

  /* crossover */
  unsigned char *r1 = pop_inst.item[pop_inst.idx[p1]].rep;
  unsigned char *r2 = pop_inst.item[pop_inst.idx[p2]].rep;

  for (int i = 0; i < pop_inst.len; i++) {
    if ((double) rand() / RAND_MAX < XOVER_RATIO) {
      gene->rep[i] = r1[i];
    }
    else {
      gene->rep[i] = r2[i];
    }
  }
}

static void select_GA(int *i) {
  double fit = (double) rand() / RAND_MAX;
  int min = 1, max = pop_inst.num, mid;

  while (min < max) {
    mid = (min + max) / 2;
    if (pop_inst.fit_cum[mid] < fit) {
      min = mid + 1;
    }
    else {
      max = mid;
    }
  }

  *i = max;
}

void mutation_GA(struct sol *gene, double decay) {
  //decay = decay * decay;
  //decay = 1.0 - (decay - 1.0) * (decay - 1.0);

  for (int i = 0; i < pop_inst.len; i++) {
    if ((double) rand() / RAND_MAX < MUTATION_RATIO * decay) {
      gene->rep[i] ^= 1;
    }
  }
}

void replace_GA(struct sol *gene, int num_replace) {
  int replace_holder[NUM_SOLUTIONS+1];
  int dist[NUM_SOLUTIONS+1];
  int min[2];

  for (int i = 1; i <= pop_inst.num; i++) {
    replace_holder[i] = -1;
  }

  for (int j = 0; j < num_replace; j++) {
    validate_sol(&gene[j]);
    min[0] = 1;
    min[1] = 1;

    for (int i = 1; i <= pop_inst.num; i++) {
      dist[i] = distance_GA(gene[j].rep,
                            pop_inst.item[pop_inst.idx[i]].rep);
      dist[i] = (dist[i] > DIST_MAX) ? MAX_VTX - dist[i] : dist[i];

      if (dist[i] < dist[min[1]]) {
        if (dist[i] < dist[min[0]]) {
          min[1] = min[0];
          min[0] = i;
        }
        else {
          min[1] = i;
        }
      }
    }

    if (replace_holder[min[0]] == -1) {
      if (gene[j].val > pop_inst.item[pop_inst.idx[min[0]]].val) {
        replace_holder[min[0]] = j;
      }
    }
    else if (replace_holder[min[1]] == -1) {
      if (gene[j].val > pop_inst.item[pop_inst.idx[min[1]]].val) {
        replace_holder[min[1]] = j;
      }
    }
    else if (gene[j].val > gene[replace_holder[min[0]]].val) {
      replace_holder[min[0]] = j;
    }
    else if (gene[j].val > gene[replace_holder[min[1]]].val) {
      replace_holder[min[1]] = j;
    }
  }

  for (int i = 1; i <= pop_inst.num; i++) {
    if (replace_holder[i] != -1) {
      pop_replace(&gene[replace_holder[i]], i);
    }
  }

  pop_sort();
}

/* print function */
void sol_print(struct sol *sol, int i) {
  printf(" [%3d] %4d  ", i, sol->val);
  for (int j = 0; j < pop_inst.len; j++) {
    printf("%d", sol->rep[j]);
  }
  printf("\n");
}

void pop_print() {
  printf("\n====== population stats ======\n\n");

  if (STATS_ENABLED) {
    printf(" Generation = %d\n", pop_inst.gen);
    printf(" Num of solutions = %d\n", pop_inst.num);
    printf(" Length of solutions = %d\n", pop_inst.len);

    double mean = pop_inst.sum_of_val / pop_inst.num;
    double variance = pop_inst.sum_of_val_square / pop_inst.num
                    - mean * mean;

    printf(" Mean = %.3lf\n", mean);
    printf(" Stderr = %.3lf\n\n", sqrt(variance));

    if (!DETAILS_ENABLED) {
      printf(" Max value = ");
      sol_print(&pop_inst.item[pop_inst.idx[1]], 1);
      printf("\n");
    }
  }

  if (DETAILS_ENABLED) {
    for (int i = 1; i <= pop_inst.num; i++) {
      sol_print(&pop_inst.item[pop_inst.idx[i]], i);
    }
    printf("\n");
  }
}

void gen_print(int i) {
  int values[NUM_SOLUTIONS];
  char info[2000];
  int n = 0;

  for (int i = 1; i <= pop_inst.num; i++) {
    values[i-1] = pop_inst.item[pop_inst.idx[i]].val;
  }
  
  int max;
  for (int i = 0; i < QNUM; i++) {
    max = INT_MIN;
    for (int j = i * QVAL; j < (i+1) * QVAL; j++) {
      max = (max < values[j]) ? values[j] : max;
    }
    n += sprintf(info + n, " %4d", max);
  }

  printf("\r%d\t%s", i, info);
  fflush(stdout);
}
