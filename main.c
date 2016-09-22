#include <stdlib.h>
#include <time.h>

#include "graph.h"
#include "funcqual.h"
#include "optim.h"
#include "clean.h"
#include "param.h"
#include <string.h>

int main(int argc,char** argv){
	unsigned i,node;
	sparse *g;
	community *com;
	cleaning *clean;
	allcoms *coms;// only used if ARG.order==1
	qualfonc quality;
	char node_coms[50],com_nodes[50],info[50],params[50];

	if(read_param(argc, argv, &ARG)==1){
		fprintf (stderr, "Usage: ./allcoms graph.txt [-qfonc char*] [-tr bool] [-qual float] [-sim float] [-max int] [-min int] [-times int] [-rep int] [-ini int] [-rm bool] [-order bool] [-path char*] [-print bool]\n");
		exit(1);
	}
	printf("qfonc=%s\n",ARG.qfonc);
	quality = choosequal(ARG.qfonc);

	strcpy(node_coms,ARG.path), strcat(node_coms,"/node_coms");
	strcpy(com_nodes,ARG.path), strcat(com_nodes,"/com_nodes");
	strcpy(info,ARG.path), strcat(info,"/info");
	strcpy(params,ARG.path), strcat(params,"/params");

	srand(time(NULL));

	if (ARG.print)
		printf("Reading edgelist from file %s\n",ARG.graph);
	g=readedgelist(ARG.graph);
	if (ARG.print)
		printf("Relabeling nodes\n");
	relabel(g);
	if (ARG.print)
		printf("Constructing the graph structure\n");
	mkgraph(g);
	if (ARG.print){
		printf("Graph loaded\n");
		printgraphinfo(g);
	}

	//allocating the community data structure for the quality function optimisation
	com=alloccom(g);

	//allocating the cleaning data structure
	clean=allocclean(g->n);

	if (ARG.order==0){
		for (i=0;i < g->n * ARG.rep;i++) {//TR
			node=rand()%g->n;//chose a random node//TR
			optimize(g,com,node,quality); //greedy optimisation of the quality function
			mkcom(com);//form the community
			com2clean(com,clean);//add the community to the cleaning datastructure
			if (ARG.print==2)
				printprogress(clean);
		}
	}
	else {
		coms=allocallcoms(g->n * ARG.rep);
		for (i=0;i < g->n * ARG.rep;i++) {//TR
			//printf("%u %e\n",i, ((double)i)/(g->n * ARG.rep));
			node=rand()%g->n;//chose a random node//TR
			optimize(g,com,node,quality); //greedy optimisation of the quality function
			mkcom(com);//form the community
			com2coms(com,coms);
		}
		printf("Sorting the found communities\n");
		sortcoms(coms);//sorting the communities in decreasing order of scores
		printf("Cleaning the found communities\n");
		for (i=0;i < coms->n;i++) {//TR
			coms2com(coms,i,com);
			com2clean(com,clean);//add the community to the cleaning datastructure
			if (ARG.print==2)
				printprogress(clean);
		}
	}

	printcoms(node_coms,com_nodes,info,clean,g);
	printparams(params);

	freegraph(g);
	return 0;
}
