#include <string.h>
#include "fm.h"

/* global environment */
extern struct graph g;
extern struct set *s;

/* local environment */
static int order[MAX_VTX+1];
static int gainv[MAX_VTX+1];
static int lockv[MAX_VTX+1];

static void compute_gainv(struct sol *e) {
  int gainu;
  char *rep = e->r;

  for (int u = 1; u <= s->len; u++) {
    gainu = 0;

    for (struct node *n = g.l[u]->head; n; n = n->next) {
      if (n->v > s->len)
        break;

      int sign = (rep[u-1] == rep[n->v-1]) * 2 - 1;
      gainu += sign * n->w;
    }

    gainv[u] = gainu;
  }
}

static int max_gainv(int i) {
  int u = 1;

  for (int j = 2; j <= s->len; j++) {
    u = (gainv[j] < gainv[u]) ? u : j;
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

    for (int i = 1; i <= s->len; i++) {
      int u = max_gainv(i);
      order[i] = u;
      lockv[u] = 1;

      for (struct node *n = g.l[u]->head; n; n = n->next) {
        if (n->v > s->len)
          break;

        if (lockv[n->v])
          continue;

        int sign = (rep[u-1] != rep[n->v-1]) * 4 - 2;
        gainv[u] += sign * n->w;
      }
    }

    k = 1;
    for (int i = 2; i <= s->len; i++) {
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
