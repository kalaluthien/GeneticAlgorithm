#include <stdio.h>

#include "common.h"
#include "parser.h"
#include "graph.h"

void parse_cmd(char *argv[], FILE **inp, FILE **outp) {
  if ((*inp = fopen(argv[1], "r")) == NULL) {
    throw("faild to open input file.\n");
  }

  if ((*outp = fopen(argv[2], "w")) == NULL) {
    throw("faild to open output file.\n");
  }
}

void parse_input(FILE *in, struct graph *g) {
  int num_vtx, num_edg;
  fscanf(in, "%d %d", &num_vtx, &num_edg);
  init_graph(g, num_vtx, num_edg);

  int u, v, w;
  while (num_edg-- > 0) {
    fscanf(in, "%d %d %d", &u, &v, &w);
    set_edge(g, u, v, w);
  }

  if (num_edg > 0) {
    throw("number of edge unmatched with input.");
  }
}
