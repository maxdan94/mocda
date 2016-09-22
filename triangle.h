#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include <stdlib.h>

#include "graph.h"

typedef struct {
	unsigned node;
	unsigned deg;
} nodedeg ;

//sparse graph structure
typedef struct {
	unsigned n;	//number of nodes
	unsigned *cd;	//cumulative degree
	unsigned *a; //list of neighbors
} sparsetr;

int compare_nodedeg(void const*, void const*);

unsigned *degrank(sparse*);

sparsetr* mkgraphtr(sparse*);

void freegraphtr(sparsetr *);

unsigned long long *trcount(sparse*);

#endif
