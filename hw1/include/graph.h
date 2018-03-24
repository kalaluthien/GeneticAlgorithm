#ifndef __GRAPH_H__
#define __GRAPH_H__

#define MAXVTX 1000
#define MAXEDG 10000

struct graph {
  int num_vtx;
  int num_edg;
  int e[MAXVTX+1][MAXVTX+1];
};

void init_graph(struct graph *g, int num_vtx, int num_edg);

#define set_edge(g, u, v, w) \
  do { \
    (g)->e[u][v] = w; \
    (g)->e[v][u] = w; \
  } while (0)

#define get_edge(g, u, v) \
  ((g)->e[u][v])

#endif
