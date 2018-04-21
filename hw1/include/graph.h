#ifndef __GRAPH_H__
#define __GRAPH_H__

#define MAX_VTX 500

struct graph {
  int num_vtx;
  int e[MAX_VTX+1][MAX_VTX+1];
};

void init_graph(struct graph *g, int num_vtx);

#endif
