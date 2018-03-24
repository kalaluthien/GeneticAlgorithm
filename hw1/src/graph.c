#include <string.h>

#include "common.h"
#include "graph.h"

void init_graph(struct graph *g, int num_vtx, int num_edg) {
  if (num_vtx > MAXVTX) {
    throw("vertex number overflowed.");
  }

  if (num_edg > MAXEDG) {
    throw("edge number overflowed.");
  }

  if (g == NULL) {
    throw("graph is null.");
  }

  memset(g, 0, sizeof(struct graph));
  g->num_vtx = num_vtx;
  g->num_edg = num_edg;
}
