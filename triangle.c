#include "triangle.h"

//For futur use in qsort.
int compare_nodedeg(void const *a, void const *b){
	nodedeg const *pa = a;
	nodedeg const *pb = b;
	return (pa->deg < pb->deg) ? 1 : -1;
}

//computing degree ordering
unsigned *degrank(sparse* g){
	unsigned i;
	nodedeg *nodedeglist=malloc(g->n*sizeof(nodedeg));
  unsigned *rank;

	for (i=0;i<g->n;i++) {
		nodedeglist[i].node=i;
		nodedeglist[i].deg=g->cd[i+1]-g->cd[i];
	}

	qsort(nodedeglist,g->n,sizeof(nodedeg),compare_nodedeg);

	rank=malloc(g->n*sizeof(unsigned));

	for (i=0;i<g->n;i++){
		rank[nodedeglist[i].node]=i;
	}
  free(nodedeglist);
  return rank;
}

sparsetr* mkgraphtr(sparse *g){
  sparsetr* gtr=malloc(sizeof(sparsetr));
  unsigned *rank=degrank(g);
  unsigned i,j,k;
  gtr->n=g->n;
  gtr->cd=malloc((g->n+1)*sizeof(unsigned));
  gtr->a=malloc((g->e)*sizeof(unsigned));
  gtr->cd[0]=0;
  for (i=0;i<g->n;i++) {
    gtr->cd[i+1]=gtr->cd[i];
    for (j=g->cd[i];j<g->cd[i+1];j++){
      k=g->a[j];
      if(rank[k]>rank[i]){
        gtr->a[gtr->cd[i+1]++]=k;
      }
    }
  }
  free(rank);
  return gtr;
}

void freegraphtr(sparsetr *gtr){
  free(gtr->cd);
  free(gtr->a);
  free(gtr);
}

//we used the algorithm Compact-Forward of Matthieu Latapy to count triangles.
unsigned long long *trcount(sparse* g){
  sparsetr* gtr=mkgraphtr(g);
  bool *tmp=calloc(gtr->n,sizeof(bool));

  unsigned long long tr=0;
  unsigned long long* trdeg=calloc(g->n,sizeof(unsigned long long));

  unsigned i,j,n1,n2,n3;
  for (n1=0;n1<gtr->n;n1++){
    for (i=gtr->cd[n1];i<gtr->cd[n1+1];i++){
      tmp[gtr->a[i]]=1;
    }
    for (i=gtr->cd[n1];i<gtr->cd[n1+1];i++){
      n2=gtr->a[i];
      for (j=gtr->cd[n2];j<gtr->cd[n2+1];j++){
        n3=gtr->a[j];
        if (tmp[n3]==1){
          tr++;
          trdeg[n1]++;
          trdeg[n2]++;
          trdeg[n3]++;
        }
      }
    }
    for (i=gtr->cd[n1];i<gtr->cd[n1+1];i++){
      tmp[gtr->a[i]]=0;
    }
  }
  free(tmp);
  freegraphtr(gtr);
  g->t=tr;
  return trdeg;
}
