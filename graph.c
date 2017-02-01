#include "graph.h"

unsigned max3(unsigned a,unsigned b,unsigned c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the weighted edgelist
sparse* readedgelist(char* edgelist){
	unsigned e1=NLINKS;
	sparse *g=malloc(sizeof(sparse));
	g->el=malloc(e1*sizeof(edge));
	FILE *file;

	g->n=0;
	g->e=0;
	file=fopen(edgelist,"r");
	if (!file) {
		fprintf (stderr, "Failed to open the file\n");
		exit(1);
	}
	while (fscanf(file,"%u %u\n", &(g->el[g->e].s), &(g->el[g->e].t))==2) {
		g->n=max3(g->n,g->el[g->e].s,g->el[g->e].t);
		if (g->e++==e1) {
			e1+=NLINKS;
			g->el=realloc(g->el,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;
	g->el=realloc(g->el,g->e*sizeof(edge));

	return g;
}

//relabel nodes from 0 to n
void relabel(sparse *g) {
	unsigned i,j;
	unsigned *newlabel;

	newlabel=malloc(g->n*sizeof(unsigned));
	for (i=0;i<g->n;i++) {
		newlabel[i]=g->n;
	}
	g->map=malloc(g->n*sizeof(unsigned));
	j=0;
	for (i=0;i<g->e;i++) {
		if (newlabel[g->el[i].s]==g->n){
			newlabel[g->el[i].s]=j;
			g->map[j++]=g->el[i].s;
		}
		if (newlabel[g->el[i].t]==g->n){
			newlabel[g->el[i].t]=j;
			g->map[j++]=g->el[i].t;
		}
		g->el[i].s=newlabel[g->el[i].s];
		g->el[i].t=newlabel[g->el[i].t];
	}
	g->n=j;
	free(newlabel);
	g->map=realloc(g->map,g->n*sizeof(unsigned));
}

//build the weighted graph structure
void mkgraph(sparse *g){
	unsigned *tmp;
	unsigned i;

	tmp=calloc(g->n,sizeof(unsigned));
	for (i=0;i<g->e;i++) {
		tmp[g->el[i].s]++;
		tmp[g->el[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+tmp[i-1];
		tmp[i-1]=0;
	}

	g->a=calloc(2*g->e,sizeof(unsigned));
	for (i=0;i<g->e;i++) {
		g->a[ g->cd[g->el[i].s] + tmp[g->el[i].s]++ ]=g->el[i].t;
		g->a[ g->cd[g->el[i].t] + tmp[g->el[i].t]++ ]=g->el[i].s;
	}
	free(tmp);
	free(g->el);
}

//free the graph stucture
void freegraph(sparse *g){
	free(g->cd);
	free(g->a);
	free(g->map);
	free(g);
}

void printgraphinfo(sparse* g){
		printf("Number of nodes: %u\n",g->n);
		printf("Number of edges: %u\n",g->e);
}
