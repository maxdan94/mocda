#ifndef PARAM_H
#define PARAM_H

#include <stdbool.h>
#include <stdio.h>

//default parametres, cannot be changed with options:
#define NLINKS 100000000 //max number of links: will increase if needed
#define NCOMS 10000000 //max number of communities: will increase if needed
#define NCPN 10 //max number of community per node: will increase if needed
#define EPS 1e-10 //minimal increase of quality for the stop criterion

//default parameters, can be changed with options:
#define QFONC "f2" //quality function choice: 1=rd, 2=phi, 3=ad, 4=ed, 5=rdtr, 6=atrd, 7=cohesion...
#define TR 0//Keep track of triangles?: 0=No,1=Yes,2=Yes and remove all nodes belonging to 0 triangles.
#define QUAL -1e10 //minimal quality to consider a community
#define SIM 0.999 //similarity threshold between two communities
#define MAX 10000 //maximum size of a found communities
#define MIN 3 //minimum size of a found communities
#define TIMES 3 //minimal number of times a set of nodes has to be found to be considered as a community
#define REP 2 //number of iterations = number of nodes times REP
#define INI 2 //initialisation of a community //1:a random node, 2:two neighbors, 3:neighborhood
#define RM 1 //authorize removal of nodes from the community
#define GREEDY 0 //0: stochastic heuristic, 1: greedy heuristic
#define ORDER 0 //order for the cleaning
//0: order the communities are found, 1: from the community of highest quality to the one of lowest quality.

//aditional parameters, can be changed with options:
#define PATH "."//path to directory in which to write the outputs
#define PRINT 2 //0=do not print, 1=print basic info, 2=print basic info and progress

typedef struct {
	char* graph;
	char* qfonc;
	char tr;
	double qual;
	double sim;
	unsigned max;
	unsigned min;
	unsigned times;
	unsigned rep;
	char ini;
	bool rm;
	bool greedy;
	bool order;
	char* path;
	char print;
}arguments;

extern arguments ARG;
extern char *optarg;
extern int optind, optopt;

int read_param(int,char**,arguments*);

void printparams(char*);

#endif
