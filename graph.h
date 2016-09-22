#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "param.h"

typedef struct {
	unsigned s;//source node
	unsigned t;//target node
} edge;

//sparse graph structure
typedef struct {
	unsigned n;	//number of nodes
	unsigned e;	//number of edges
	unsigned *cd;	//cumulative degree
	unsigned *a; //list of neighbors
	edge *el;//edge list
	unsigned *map;//maping: map[newID] = oldID;

	unsigned long long t;	//number of triangles//TR
} sparse;

//compute the maximum of three unsigned
unsigned max3(unsigned,unsigned,unsigned);

//reading the edgelist from file and storing t in the sparse
sparse* readedgelist(char*);

//relabeling nodes from 0 to number of nodes
void relabel(sparse*);

//constructing the sparse graph datastructure
void mkgraph(sparse*);

//free memory
void freegraph(sparse*);

//printing information about the graph
void printgraphinfo(sparse*);

#endif
