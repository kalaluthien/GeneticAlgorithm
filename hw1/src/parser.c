#include <stdio.h>

#include "common.h"
#include "parser.h"
#include "graph.h"

void parse_cmd(char *argv[], FILE **inp, FILE **outp) {
  throw(!(*inp = fopen(argv[1], "r")), "faild to open input file.\n");
  throw(!(*outp = fopen(argv[2], "w")), "faild to open output file.\n");
}

void parse_input(FILE *in, struct graph *g) {
  int num_vtx, num_edg;
  fscanf(in, "%d %d", &num_vtx, &num_edg);
  init_graph(g, num_vtx);

  int u, v, w;
  while (num_edg--) {
    fscanf(in, "%d %d %d", &u, &v, &w);

    if (u < v) {
      g->e[u][v] = w;
    }
    else {
      g->e[v][u] = w;
    }
  }
}
