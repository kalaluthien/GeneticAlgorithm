#include <stdio.h>

#include "common.h"
#include "parser.h"
#include "graph.h"

void parse_input(FILE *in, struct graph *g) {
  int num_vtx, num_edg;

  fscanf(in, "%d %d", &num_vtx, &num_edg);
  init_graph(g, num_vtx);

  int u, v, w;

  while (num_edg--) {
    fscanf(in, "%d %d %d", &u, &v, &w);
    insert_graph(g, u, v, w);
  }
}
