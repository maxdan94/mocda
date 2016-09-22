#ifndef FUNCQUAL_H
#define FUNCQUAL_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graph.h"

typedef struct {
	unsigned n; //number of nodes in the graph (=sizes of in, bor and inlist and max sizes of list)
 	unsigned e; //number of edges in the graph
	unsigned lin; //number of links inside the community
	unsigned lout; //number of links going out of the community
	unsigned s; //number of nodes in the community

	unsigned long long tr; //number of triangles in the graph
	unsigned long long tr3; //number of triangles with 3 nodes in the community//TR
	unsigned long long tr2; //number of triangles with exactly 2 nodes in the community//TR
	unsigned long long tr1; //number of triangles with exactly 1 node in the community//TR
} features;

typedef double (*qualfonc)(features*);

//allocating the features datastructure
features *allocfeat(sparse*);

//relative density
double rd(features*);

//conductance
double phi(features*);

//average degree
double ad(features*);

//single edit distance
double ed(features*);

double rdt(features*);//TR
double atd(features*);//TR
double cohesion(features*);//TR

double f1(features*);
double f2(features*);
double f3(features*);
double f4(features*);

double g1(features*);
double g2(features*);
double g3(features*);
double g4(features*);
double g5(features*);

qualfonc choosequal(char*);

#endif
