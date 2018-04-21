#include <string.h>

#include "common.h"
#include "graph.h"

void init_graph(struct graph *g, int num_vtx) {
  throw(!g, "graph is null.");
  throw(num_vtx > MAX_VTX, "vertex number overflowed.");

  memset(g, 0, sizeof(struct graph));
  g->num_vtx = num_vtx;
}
