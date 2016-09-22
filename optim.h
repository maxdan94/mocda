#ifndef OPTIM_H
#define OPTIM_H

#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"
#include "param.h"
#include "funcqual.h"
#include "triangle.h"

typedef struct {
	bool *in; //in[i]=1 iff i in the community, 0 otherwise.
	unsigned *bor; //bor[i]=number of neighbors in the community
	unsigned *list; //nodes that are or were recently in the community or at the border
	bool *inlist; //inlist[i]=1 iff i is in list.
	unsigned nlist; //number of nodes in list
	unsigned *deg; //degrees of nodes

	unsigned long long *tr2bor; //tr2bor[i]= number of triangles containing i and two other nodes in the community//TR
	unsigned long long *tr1bor; //tr1bor[i]= number of triangles containing i and exactly one other node in the community//TR
	unsigned long long *trdeg; //trdeg[i]= number of triangles containing i//TR

	unsigned n; //number of nodes in the graph (=sizes of in, bor and inlist and max sizes of list)
	double qual; //quality of the community
	unsigned node; //starting edge//TR

	features* feat;//considered features of the community
	features* tmpfeat;//used to check increase of quality

	unsigned *nodes;//nodes in the final community

	bool* tmp;//helper to compute intersections for triangles//TR

} community;

//allocating the community datastructure for the optimisation
community *alloccom(sparse*);

void empty(community*);

//initialize the datastructure with the input node
void inicom1(community*,sparse*,unsigned,qualfonc);
void inicom2(community*,sparse*,unsigned,qualfonc);
void inicomN(community*,sparse*,unsigned,qualfonc);
void inicom(community*,sparse*,unsigned,qualfonc);

//Check if removing "node" increases the quality and remove it if it does.
double check_rm(community*,unsigned,qualfonc);
void update_rm(community*,sparse*,unsigned,qualfonc);

//Check if adding "node" increases the quality and add it if it does.
double check_add(community*,unsigned,qualfonc);
void update_add(community*,sparse*,unsigned,qualfonc);

//greedy optimisation
void optimize(sparse*,community*,unsigned,qualfonc);

//make the community.
void mkcom(community*);

#endif
