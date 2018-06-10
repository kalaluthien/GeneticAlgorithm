#include <string.h>

#include "common.h"
#include "graph.h"

static void insert_list(struct list *l, int v, int w) {
  struct node *n = (struct node *) malloc(sizeof(struct node));

  if (l->head == NULL) {
    l->head = n;
    l->tail = l->head;
  }
  else {
    l->tail->next = n;
    l->tail = l->tail->next;
  }

  l->tail->v = v;
  l->tail->w = w;

  l->size++;
}

static void init_graph(struct graph *g, int num_vtx) {
  throw(!g, "graph is null.");
  throw(num_vtx > MAX_VTX, "vertex number overflowed.");

  g->num_vtx = num_vtx;
  g->l[0] = NULL;

  for (int u = 1; u <= num_vtx; u++) {
    g->l[u] = (struct list *) malloc(sizeof(struct list));
    g->l[u]->size = 0;
    g->l[u]->head = NULL;
    g->l[u]->tail = NULL;
  }
}

void create_graph(struct graph *g, FILE *in) {
  int num_vtx, num_edg;

  fscanf(in, "%d %d", &num_vtx, &num_edg);
  init_graph(g, num_vtx);

  int u, v, w;

  while (num_edg--) {
    fscanf(in, "%d %d %d", &u, &v, &w);
    insert_list(g->l[u], v, w);
    insert_list(g->l[v], u, w);
  }
}
