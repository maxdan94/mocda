#include "clean.h"

//Allocating memory for the cleaning datastructure
cleaning *allocclean(unsigned n){
	cleaning *clean=malloc(sizeof(cleaning));
	clean->n=n;
	clean->c=calloc(n,sizeof(unsigned*));
	clean->s=calloc(n,sizeof(unsigned));
	clean->smax=calloc(n,sizeof(unsigned));

	clean->nc=0;
	clean->mc=NCOMS;
	clean->size=malloc(clean->mc*sizeof(unsigned));
	clean->lin=malloc(clean->mc*sizeof(unsigned));
	clean->lout=malloc(clean->mc*sizeof(unsigned));

	if (ARG.tr){
		clean->tr3=malloc(clean->mc*sizeof(unsigned long long));//TR
		clean->tr2=malloc(clean->mc*sizeof(unsigned long long));//TR
		clean->tr1=malloc(clean->mc*sizeof(unsigned long long));//TR
	}
	else{
		clean->tr3=NULL;
		clean->tr2=NULL;
		clean->tr1=NULL;
	}

	clean->qual=malloc(clean->mc*sizeof(double));
	clean->times=malloc(clean->mc*sizeof(unsigned));
	clean->node=malloc(clean->mc*sizeof(unsigned));
	clean->com=malloc(clean->mc*sizeof(unsigned*));

	clean->tmp=calloc(clean->mc,sizeof(unsigned));
	clean->list=calloc(clean->mc,sizeof(unsigned));
	clean->nlist=0;

	clean->itc=0;
	clean->succ=0;
	clean->qualc=0;
	clean->difc=0;
	clean->selc=0;
	return clean;
}

//Looking for the most similar already found community
void comsim(community* comopt,cleaning *clean){
	double sim2;
	unsigned i,j,node,com,s=(comopt->feat)->s;
	clean->sim=0;
	clean->isim=0;

	for (i=0;i<s;i++){
		node=comopt->nodes[i];
		for (j=0;j<clean->s[node];j++){
			if (clean->tmp[clean->c[node][j]]==0){
				clean->list[clean->nlist++]=clean->c[node][j];
			}
			clean->tmp[clean->c[node][j]]++;
		}
	}

	for (i=0;i<clean->nlist;i++){
		com=clean->list[i];
		sim2=((double)(clean->tmp[com]))/(s+clean->size[com]);
		if (sim2>=clean->sim){
			clean->sim=sim2;
			clean->isim=com;
		}
		clean->tmp[com]=0;
	}
	clean->nlist=0;
	clean->sim*=2;
}

//adding the community to the cleaning datastructure
void com2clean(community* comopt,cleaning* clean){
	unsigned i,node,s;

	clean->itc++;
	if ((comopt->feat)->s > ARG.max){
		empty(comopt);
		return;
	}
	if ((comopt->feat)->s < ARG.min){
		empty(comopt);
		return;
	}
	clean->succ++;
	if (comopt->qual<ARG.qual){
		empty(comopt);
		return;
	}
	clean->qualc++;

	comsim(comopt,clean);

	if (clean->sim<ARG.sim){
		clean->difc++;
		if (clean->nc==clean->mc){
			clean->mc+=NCOMS;
			clean->size=realloc(clean->size,clean->mc*sizeof(unsigned));
			clean->lin=realloc(clean->lin,clean->mc*sizeof(unsigned));
			clean->lout=realloc(clean->lout,clean->mc*sizeof(unsigned));
			if (ARG.tr){
				clean->tr3=realloc(clean->tr3,clean->mc*sizeof(unsigned long));//TR
				clean->tr2=realloc(clean->tr2,clean->mc*sizeof(unsigned long));//TR
				clean->tr1=realloc(clean->tr1,clean->mc*sizeof(unsigned long));//TR
			}
			clean->qual=realloc(clean->qual,clean->mc*sizeof(double));
			clean->node=realloc(clean->times,clean->mc*sizeof(unsigned));
			clean->times=realloc(clean->times,clean->mc*sizeof(unsigned));
			clean->tmp=realloc(clean->tmp,clean->mc*sizeof(unsigned));
			clean->list=realloc(clean->list,clean->mc*sizeof(unsigned));
			clean->com=realloc(clean->com,clean->mc*sizeof(unsigned*));
			for (i=clean->mc-NCOMS;i<clean->mc;i++){
				clean->tmp[i]=0;
			}
		}
		s=(comopt->feat)->s;
		clean->size[clean->nc]=s;
		clean->lin[clean->nc]=(comopt->feat)->lin;
		clean->lout[clean->nc]=(comopt->feat)->lout;
		if (ARG.tr){
			clean->tr3[clean->nc]=(comopt->feat)->tr3;//TR
			clean->tr2[clean->nc]=(comopt->feat)->tr2;//TR
			clean->tr1[clean->nc]=(comopt->feat)->tr1;//TR
		}
		clean->qual[clean->nc]=comopt->qual;
		clean->node[clean->nc]=comopt->node;//TR
		clean->times[clean->nc]=1;
		clean->com[clean->nc]=malloc(s*sizeof(unsigned));
		for (i=0;i<s;i++){
			node=comopt->nodes[i];
			clean->com[clean->nc][i]=node;
			if (clean->s[node]==clean->smax[node]){
				if (clean->s[node]==0){
					clean->c[node]=malloc(NCPN*sizeof(unsigned));
					clean->smax[node]=NCPN;
				}
				else {
					clean->smax[node]+=NCPN;
					clean->c[node]=realloc(clean->c[node],clean->smax[node]*sizeof(unsigned));
				}
			}
			clean->c[node][clean->s[node]++]=clean->nc;
		}
		clean->nc++;
		empty(comopt);
		return;
	}

	clean->times[clean->isim]++;

	if (clean->times[clean->isim]==ARG.times){
		clean->selc++;
	}
	empty(comopt);
}

void printcoms(char* node_coms,char* com_nodes,char* info,cleaning* clean,sparse* g){
	unsigned i,j,k=0;
	FILE *file, *file2;
	unsigned* tmp=malloc(clean->nc*sizeof(unsigned));

	file=fopen(com_nodes,"w");
	file2=fopen(info,"w");
	for (i=0;i<clean->nc;i++) {
		if (clean->times[i]>=ARG.times){
			for (j=0;j<clean->size[i]-1;j++) {
				fprintf(file,"%u ",g->map[clean->com[i][j]]);
			}
			fprintf(file,"%u\n",g->map[clean->com[i][j]]);
			if (ARG.tr){
				fprintf(file2,"%u %u %u %u %u %llu %llu %llu %le\n",clean->size[i],g->map[clean->node[i]],clean->times[i],clean->lin[i],clean->lout[i],clean->tr3[i],clean->tr2[i],clean->tr1[i],clean->qual[i]);
			}
			else{
				fprintf(file2,"%u %u %u %u %u -1 -1 -1 %le\n",clean->size[i],g->map[clean->node[i]],clean->times[i],clean->lin[i],clean->lout[i],clean->qual[i]);
			}
			tmp[i]=k++;
		}
	}
	fclose(file);
	fclose(file2);

	file=fopen(node_coms,"w");
	for (i=0;i<clean->n;i++) {
		fprintf(file,"%u",g->map[i]);
		for (j=0;j<clean->s[i];j++) {
			if (clean->times[clean->c[i][j]]>=ARG.times){
				fprintf(file," %u",tmp[clean->c[i][j]]);
		 	}
		}
		fprintf(file,"\n");
	}
	fclose(file);
	free(tmp);
}

//printing progress every 1%
void printprogress(cleaning* clean){
	if (((clean->n>100000) && ((clean->itc*100)/(ARG.rep*clean->n) != ((clean->itc-1)*100)/(ARG.rep*clean->n))) || (clean->itc==clean->n*ARG.rep) ){
		printf("\nProgress: %.0f percent\n",clean->itc*100./(clean->n*ARG.rep));
		printf("Number of optimisation: %u\n",clean->itc);
		printf("Number of succesfull optimisation: %u\n",clean->succ);
		printf("Number of good quality community: %u\n",clean->qualc);
		printf("Number of different communities found: %u\n",clean->difc);
		printf("Number of communities found: %u\n",clean->selc);
	}
}

//used is ARG.order=1

//allocating the allcoms structure
allcoms *allocallcoms(unsigned n){
	allcoms *coms=malloc(sizeof(allcoms));
	coms->n=0;
	coms->feat=malloc(n*sizeof(features));
	coms->nodes=malloc(n*sizeof(unsigned*));
	coms->sort=malloc(n*sizeof(idval));
	return coms;
}

void com2coms(community* com,allcoms* coms){
	unsigned i=(coms->n)++,j,s=com->feat->s;
	coms->feat[i].n=com->feat->n;
	coms->feat[i].e=com->feat->e;
	coms->feat[i].lin=com->feat->lin;
	coms->feat[i].lout=com->feat->lout;
	coms->feat[i].s=s;
	coms->sort[i].id=i;
	coms->sort[i].val=com->qual;
	if (ARG.tr){
		coms->feat[i].tr=com->feat->tr;//TR
		coms->feat[i].tr3=com->feat->tr3;//TR
		coms->feat[i].tr2=com->feat->tr2;//TR
		coms->feat[i].tr1=com->feat->tr1;//TR
	}
	coms->nodes[i]=malloc(s*sizeof(unsigned));
	for (j=0;j<s;j++){
		coms->nodes[i][j]=com->nodes[j];
	}
	empty(com);
}

static int compare(void const *a, void const *b){
	idval const *pa = a;
	idval const *pb = b;
	if ((*pa).val<=(*pb).val)
		return 1;
	return -1;
}

//sorting the communities in decreasing order of scores
void sortcoms(allcoms* coms){
	qsort(coms->sort,coms->n,sizeof(idval),compare);
}

void coms2com(allcoms* coms,unsigned i,community* com){
	unsigned j=coms->sort[i].id,k,s;
	s=coms->feat[j].s;
	com->feat=&(coms->feat[j]);
	com->qual=coms->sort[i].val;
	for (k=0;k<s;k++){
		com->nodes[k]=coms->nodes[j][k];
	}
}
