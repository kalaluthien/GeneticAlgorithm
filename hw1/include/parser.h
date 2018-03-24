#ifndef __PARSER_H__
#define __PARSER_H__

#include "graph.h"

void parse_cmd(char *argv[], FILE **inp, FILE **outp);
void parse_input(FILE *in, struct graph *g);

#endif
