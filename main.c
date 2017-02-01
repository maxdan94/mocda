#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "graph.h"
#include "funcqual.h"
#include "optim.h"
#include "clean.h"
#include "param.h"

//printing time
void printtime(char* runtime,time_t* t){
	FILE *file=fopen(runtime,"w");
	fprintf(file,"Time to read the edge list: %ldh%ldm%lds\n",(t[1]-t[0])/3600,((t[1]-t[0])%3600)/60,((t[1]-t[0])%60));
	fprintf(file,"Time to build the datastructures: %ldh%ldm%lds\n",(t[2]-t[1])/3600,((t[2]-t[1])%3600)/60,((t[2]-t[1])%60));
	fprintf(file,"Time for optimisation and cleaning: %ldh%ldm%lds\n",(t[3]-t[2])/3600,((t[3]-t[2])%3600)/60,((t[3]-t[2])%60));
	fprintf(file,"Overall time (minus I/O): %ldh%ldm%lds\n",(t[3]-t[1])/3600,((t[3]-t[1])%3600)/60,((t[3]-t[1])%60));
	fprintf(file,"Overall time: %ldh%ldm%lds\n",(t[4]-t[0])/3600,((t[4]-t[0])%3600)/60,((t[4]-t[0])%60));
	fclose(file);
}

//printing summary of the running%
void printsummary(char* summary,cleaning* clean){
	FILE *file=fopen(summary,"w");
	fprintf(file,"Number of optimisation: %u\n",clean->itc);
	fprintf(file,"Number of succesfull optimisation: %u\n",clean->succ);
	fprintf(file,"Number of good quality community: %u\n",clean->qualc);
	fprintf(file,"Number of different communities found: %u\n",clean->difc);
	fprintf(file,"Number of communities found: %u\n",clean->selc);
	fclose(file);
}


int main(int argc,char** argv){
	unsigned i,node;
	sparse *g;
	community *com;
	cleaning *clean;
	allcoms *coms;// only used if ARG.order==1
	qualfonc quality;
	char node_coms[100],com_nodes[100],info[100],params[100],runtime[100],summary[100];
	time_t t[5];

	if(read_param(argc, argv, &ARG)==1){
		fprintf (stderr, "Usage: ./mocda graph.txt [-qfonc char*] [-tr bool] [-qual float] [-sim float] [-max int] [-min int] [-times int] [-rep int] [-ini int] [-rm bool] [-order bool] [-path char*] [-print bool]\n");
		exit(1);
	}

	quality = choosequal(ARG.qfonc);

	strcpy(node_coms,ARG.path), strcat(node_coms,"/node_coms");
	strcpy(com_nodes,ARG.path), strcat(com_nodes,"/com_nodes");
	strcpy(info,ARG.path), strcat(info,"/info");
	strcpy(params,ARG.path), strcat(params,"/params");
	strcpy(runtime,ARG.path), strcat(runtime,"/runtime");
	strcpy(summary,ARG.path), strcat(summary,"/summary");


	srand(time(NULL));

	if (ARG.print)
		printf("Reading edgelist from file %s\n",ARG.graph);

	t[0]=time(NULL);
	g=readedgelist(ARG.graph);
	t[1]=time(NULL);

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

	t[2]=time(NULL);

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
		if (ARG.print){
			printf("Sorting the found communities\n");
		}
		sortcoms(coms);//sorting the communities in decreasing order of scores
		if (ARG.print){
			printf("Cleaning the found communities\n");
		}
		for (i=0;i < coms->n;i++) {//TR
			coms2com(coms,i,com);
			com2clean(com,clean);//add the community to the cleaning datastructure
			if (ARG.print==2)
				printprogress(clean);
		}
	}
	t[3]=time(NULL);

	printcoms(node_coms,com_nodes,info,clean,g);
	freegraph(g);

	printsummary(summary,clean);
	printparams(params);

	t[4]=time(NULL);

	printtime(runtime,t);

	return 0;
}
