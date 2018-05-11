#ifndef __GRAPH_H__
#define __GRAPH_H__

#define MAX_VTX 1000

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

struct graph {
  int num_vtx;
  struct list *l[MAX_VTX+1];
};

void init_graph(struct graph *g, int num_vtx);
void insert_graph(struct graph *g, int u, int v, int w);

#endif
