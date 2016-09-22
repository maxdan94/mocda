#ifndef CLEAN_H
#define CLEAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "param.h"
#include "optim.h"
#include "graph.h"

typedef struct {
	unsigned n;//number of nodes
	unsigned **c;//c[i]=communities containing node i.
	unsigned *s;//s[i]=number of communities containing node i.
	unsigned *smax;//smax[i]=max number of communities

	unsigned nc;//number of communities
	unsigned mc;//max number of communities
	unsigned *size;//size[i]=size of the community i
	unsigned *lin;//lin[i]=number of inside links of the community i
	unsigned *lout;//lout[i]=number of outside links of the community i
	unsigned long long *tr3;//tr3[i]==number of triangle with 3 nodes in the community i//TR
	unsigned long long *tr2;//tr2[i]=number of triangle with exactly 2 nodes in the community i//TR
	unsigned long long *tr1;//tr1[i]=number of triangle with exactly 1 node in the community i//TR
	double *qual;//qual[i]=quality of the community i
	unsigned *times;//times[i]=number of times the community i was found
	unsigned *node;//first otimisation leading to community i started from node[i]//TR
	unsigned **com;//com[i]=nodes in community i

	unsigned *tmp;//tmp[i] number of nodes shared by the current community and community i
	unsigned *list;//list of the community sharing at least one node with the current community
	unsigned nlist;//length of list

	double sim;//similarity of the curent community to the most similar already found
	unsigned isim;//index of this most similar community

	unsigned itc;//total number of optimisation
	unsigned succ;//number of succesfull optimisation
	unsigned qualc;//number of communities of good quality
	unsigned difc;//number of different communities of good quality
	unsigned selc;//number of different communities of good quality found more than NTIME

} cleaning;

//allocating memory for the cleaning datastructure
cleaning *allocclean(unsigned);

//compute the similarity (F-score) of the current community to the most similar already found community
void comsim(community*,cleaning*);

//add the found community to the cleaning datastructure
void com2clean(community*,cleaning*);

//printing all communities and informations in the files
void printcoms(char*,char*,char*,cleaning*,sparse*);

//printing in terminal the progress
void printprogress(cleaning*);

//used is ARG.order=1

//compact community structure
typedef struct {
	unsigned id;
	double val;
} idval;
typedef struct {
	unsigned n;//number of communities;
	features* feat;//considered features of the communities
	unsigned** nodes;//nodes in the final communities
	idval * sort;
} allcoms;

//allocating the allcoms structure
allcoms *allocallcoms(unsigned);

//storing the found community and its features
void com2coms(community*,allcoms*);

//for futur use in qsort
static int compare(void const *, void const *);

void sortcoms(allcoms*);

//copying the community coms[i] in com
void coms2com(allcoms*,unsigned,community*);


#endif
