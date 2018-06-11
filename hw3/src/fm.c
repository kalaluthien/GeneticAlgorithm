#include <string.h>
#include "fm.h"

/* global environment */
extern struct graph g;
extern struct set *s;

/* local environment */
static int order[MAX_VTX+1];
static int gainv[MAX_VTX+1];
static int lockv[MAX_VTX+1];

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

static void compute_gainv(struct sol *e) {
  for (int i = 1; i <= s->len; i++) {
    gainv[i] = gain(e, i);
  }
}

static int max_gainv() {
  int u = 1;

  for (int i = 2; i <= s->len; i++) {
    u = (gainv[i] < gainv[u] || lockv[i] == 1) ? u : i;
  }

  return u;
}

void max_fm(struct sol *e) {
  int k, improved;
  char *rep = e->r;

  do {
    improved = 0;
    compute_gainv(e);
    memset(lockv, 0, sizeof(lockv));

    for (int i = 1; i < s->len; i++) {
      int u = max_gainv();
      int c = rep[u-1];

      int *vp = g.v[u];
      int *wp = g.w[u];
      int size = g.size[u];

      order[i] = u;
      lockv[u] = 1;

      for (int j = 0; j < size; j++) {
        int v = vp[j];

        if (v > s->len || lockv[v] == 1)
          continue;

        gainv[v] += ((c != rep[v-1]) * 4 - 2) * wp[j];
      }
    }

    k = 1;
    for (int i = 2; i < s->len; i++) {
      gainv[order[i]] += gainv[order[i-1]];
      k = (gainv[order[i]] < gainv[order[k]]) ? k : i;
    }

    if (gainv[order[k]] > 0) {
      improved = 1;

      for (int i = 1; i <= k; i++) {
        rep[order[i]] ^= 1;
      }
      e->v += gainv[order[k]];
    }
  } while (improved);
}
