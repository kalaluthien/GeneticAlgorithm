#include <string.h>
#include "common.h"
#include "graph.h"

struct node {
  int v;
  int w;
  struct node *next;
};

struct list {
  int size;
  struct node *head;
  struct node *tail;
};

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

static struct list *create_list() {
  struct list *l  = (struct list *) malloc(sizeof(struct list));
  l->size = 0;
  l->head = NULL;
  l->tail = NULL;

  return l;
}

static void release_list(struct list *l) {
  struct node *n = l->head, *t = NULL;

  while (n != NULL) {
    t = n;
    n = n->next;
    free(t);
  }
}

void create_graph(struct graph *g, FILE *in) {
  static struct list *l[MAX_VTX+1];

  int i, u, v, w, num_vtx, num_edg;

  fscanf(in, "%d %d", &num_vtx, &num_edg);

  throw(!g, "graph is null.");
  throw(num_vtx > MAX_VTX, "vertex number overflowed.");

  g->num_vtx = num_vtx;

  for (u = 1; u <= num_vtx; u++) {
    l[u] = create_list();
  }

  while (num_edg--) {
    fscanf(in, "%d %d %d", &u, &v, &w);
    insert_list(l[u], v, w);
    insert_list(l[v], u, w);
  }

  for (u = 1; u <= num_vtx; u++) {
    g->v[u] = (int *) malloc(sizeof(int) * l[u]->size);
    g->w[u] = (int *) malloc(sizeof(int) * l[u]->size);
    g->size[u] = l[u]->size;

    i = 0;
    for (struct node *n = l[u]->head; n != NULL; n = n->next) {
      g->v[u][i] = n->v;
      g->w[u][i] = n->w;
      i++;
    }
  }

  for (u = 1; u <= num_vtx; u++) {
    release_list(l[u]);
  }
}
