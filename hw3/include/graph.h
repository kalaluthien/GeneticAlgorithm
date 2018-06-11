#ifndef __GRAPH_H__
#define __GRAPH_H__

#define MAX_VTX 3000

struct graph {
  int num_vtx;
  int *v[MAX_VTX+1];
  int *w[MAX_VTX+1];
  int size[MAX_VTX+1];
};

void create_graph(struct graph *g, FILE *in);

#endif
