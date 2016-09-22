#include "optim.h"

community *alloccom(sparse *g){
	community *com;
	unsigned i;
	com=malloc(sizeof(community));
	com->n=g->n;
	com->in=calloc(com->n,sizeof(bool));
	com->bor=calloc(com->n,sizeof(unsigned));
	com->inlist=calloc(com->n,sizeof(bool));
	com->list=malloc(com->n*sizeof(unsigned));
	com->deg=malloc(com->n*sizeof(unsigned));
	com->nlist=0;
	for (i=0;i<com->n;i++) {
		com->deg[i]=g->cd[i+1]-g->cd[i];
	}
	if (ARG.tr){
		com->tr2bor=calloc(com->n,sizeof(unsigned long long));//TR
		com->tr1bor=calloc(com->n,sizeof(unsigned long long));//TR
		com->trdeg=trcount(g);//TR
		com->tmp=calloc(com->n,sizeof(bool));
	}
	else{
		com->tr2bor=NULL;//TR
		com->tr1bor=NULL;//TR
		com->trdeg=NULL;//TR
		com->tmp=NULL;
	}
	com->feat=allocfeat(g);
	com->tmpfeat=allocfeat(g);

	com->nodes=malloc(com->n*sizeof(unsigned));

	return com;
}

//empty the community
void empty(community *com){
	unsigned i,node;
	(com->feat)->lin=0;
	(com->feat)->lout=0;
	(com->feat)->s=0;
	if (ARG.tr){
		for (i=0;i<com->nlist;i++) {
			node=com->list[i];
			com->inlist[node]=0;
			com->in[node]=0;
			com->bor[node]=0;
			com->tr1bor[node]=0;//TR
			com->tr2bor[node]=0;//TR
		}
		(com->feat)->tr3=0;
		(com->feat)->tr2=0;
		(com->feat)->tr1=0;
	}
	else{
		for (i=0;i<com->nlist;i++) {
			node=com->list[i];
			com->inlist[node]=0;
			com->in[node]=0;
			com->bor[node]=0;
		}
	}
	com->nlist=0;
}

//initialised the community with the single node "node".
void inicom1(community *com, sparse *g, unsigned node,qualfonc quality){
	check_add(com,node,quality);
	update_add(com,g,node,quality);
}

//initialised the community with the node "node" and a random neighbor.//TR
void inicom2(community *com, sparse *g, unsigned node, qualfonc quality){
	unsigned i,neigh;
	check_add(com,node,quality);
	update_add(com,g,node,quality);
	i=rand() % (com->deg[node]);
	neigh=g->a[g->cd[node]+i];
	check_add(com,neigh,quality);
	update_add(com,g,neigh,quality);
}

//initialised the community with the node "node" and all its neighbors.//TR
void inicomN(community *com, sparse *g, unsigned node, qualfonc quality){
	unsigned i,neigh;
	check_add(com,node,quality);
	update_add(com,g,node,quality);
	for (i=g->cd[node];i<g->cd[node+1];i++){
		neigh=g->a[i];
		check_add(com,neigh,quality);
		update_add(com,g,neigh,quality);
	}
}

void inicom(community *com, sparse *g, unsigned node, qualfonc quality){
	com->node=node;
	switch (ARG.ini){
		case 1:
			inicom1(com, g, node, quality);
			break;
		case 2:
			inicom2(com, g, node, quality);
			break;
		default:
			inicomN(com, g, node, quality);
			break;
	}
}

// randomize the order of the nodes in list (Fisher–Yates shuffle)
void randomize(community *com) {
	unsigned i,j,temp;
	for (i=com->nlist-1;i>1;i--) {
		j=rand() % i;
		temp=com->list[i];
		com->list[i]=com->list[j];
		com->list[j]=temp;
	}
}

//Check if removing "node" increases the quality
double check_rm(community* com,unsigned node,qualfonc quality){
	//removing node from the community would lead to:
	(com->tmpfeat)->lin=(com->feat)->lin - com->bor[node];
	(com->tmpfeat)->lout=(com->feat)->lout + 2*com->bor[node] - com->deg[node];
	if (ARG.tr){
		(com->tmpfeat)->tr3=(com->feat)->tr3 - com->tr2bor[node];//TR
		(com->tmpfeat)->tr2=(com->feat)->tr2 + com->tr2bor[node] - com->tr1bor[node];//TR
		(com->tmpfeat)->tr1=(com->feat)->tr1 - com->trdeg[node] + 2*com->tr1bor[node] + com->tr2bor[node];//TR
	}
	(com->tmpfeat)->s=(com->feat)->s - 1;
	return quality(com->tmpfeat);
}

//Remove "node" from the community if it increases the quality.
void update_rm(community* com,sparse* g,unsigned node, qualfonc quality){
	unsigned i,j,neigh,neigh2;//TR
	double qual=quality(com->tmpfeat);
	//if quality increases remove "node" from the community:
	com->qual=qual;
	(com->feat)->lin=(com->tmpfeat)->lin;
	(com->feat)->lout=(com->tmpfeat)->lout;
	((com->feat)->s)--;
	// remove "node" from the community
	com->in[node]=0;
	// decrease by one the number of neighbors in the community of the neighbors of "node"
	for (i=g->cd[node];i<g->cd[node+1];i++){
		com->bor[g->a[i]]--;
	}

	if (ARG.tr){
		(com->feat)->tr3=(com->tmpfeat)->tr3;//TR
		(com->feat)->tr2=(com->tmpfeat)->tr2;//TR
		(com->feat)->tr1=(com->tmpfeat)->tr1;//TR
		for (i=g->cd[node];i<g->cd[node+1];i++){
			com->tmp[g->a[i]]=1;//TR
		}
		for (i=g->cd[node];i<g->cd[node+1];i++){
			neigh=g->a[i];
			if (com->in[neigh]==1){
				for (j=g->cd[neigh];j<g->cd[neigh+1];j++){
					neigh2=g->a[j];
					if (com->tmp[neigh2]){
						com->tr2bor[neigh2]--;
						com->tr1bor[neigh2]++;
					}
				}
			}
			else{
				for (j=g->cd[neigh];j<g->cd[neigh+1];j++){
					neigh2=g->a[j];
					if (com->tmp[neigh2]){
						com->tr1bor[neigh2]--;
					}
				}
			}
		}
		for (i=g->cd[node];i<g->cd[node+1];i++){
			com->tmp[g->a[i]]=0;
		}
	}
}

//Check if adding "node" increases the quality
double check_add(community* com,unsigned node,qualfonc quality){//TR
	// adding "node" to the community would lead to:
	(com->tmpfeat)->lin=(com->feat)->lin + com->bor[node];
	(com->tmpfeat)->lout=(com->feat)->lout + com->deg[node] - 2*com->bor[node];
	(com->tmpfeat)->s=(com->feat)->s + 1;
	if (ARG.tr){
		(com->tmpfeat)->tr3 = (com->feat)->tr3 + com->tr2bor[node];//TR
		(com->tmpfeat)->tr2 = (com->feat)->tr2 - com->tr2bor[node] + com->tr1bor[node];//TR
		(com->tmpfeat)->tr1 = (com->feat)->tr1 + com->trdeg[node] - 2*com->tr1bor[node] - com->tr2bor[node];//TR
	}
	return quality(com->tmpfeat);
}

//Add "node" to the community if it increases the quality.
void update_add(community* com,sparse* g,unsigned node, qualfonc quality){
	unsigned i,j,neigh,neigh2;
	double qual=quality(com->tmpfeat);

	com->qual=qual;
	(com->feat)->lin=(com->tmpfeat)->lin;
	(com->feat)->lout=(com->tmpfeat)->lout;
	((com->feat)->s)++;
	//add "node" to the community
	com->in[node]=1;
	// increase by one the number of neighbors in the community of the neighbors of "node" and add it to borlist if needed
	for (i=g->cd[node];i<g->cd[node+1];i++) {
		neigh=g->a[i];
		// add "neigh" to list if needed
		if (com->inlist[neigh]==0){
			com->list[(com->nlist)++]=neigh;
			com->inlist[neigh]=1;
		}
		com->bor[neigh]++;
	}
	//update tr1bor and tr2bor using tmp.//TR
	if(ARG.tr){
		(com->feat)->tr3=(com->tmpfeat)->tr3;//TR
		(com->feat)->tr2=(com->tmpfeat)->tr2;//TR
		(com->feat)->tr1=(com->tmpfeat)->tr1;//TR
		for (i=g->cd[node];i<g->cd[node+1];i++) {
			com->tmp[g->a[i]]=1;
		}
		for (i=g->cd[node];i<g->cd[node+1];i++){
			neigh=g->a[i];
			if (com->in[neigh]==1){
				for (j=g->cd[neigh];j<g->cd[neigh+1];j++){
					neigh2=g->a[j];
					if (com->tmp[neigh2]){
						com->tr2bor[neigh2]++;
						com->tr1bor[neigh2]--;
					}
				}
			}
			else{
				for (j=g->cd[neigh];j<g->cd[neigh+1];j++){
					neigh2=g->a[j];
					if (com->tmp[neigh2]){
						com->tr1bor[neigh2]++;
					}
				}
			}
		}
		for (i=g->cd[node];i<g->cd[node+1];i++){
			com->tmp[g->a[i]]=0;
		}
	}
}

//for each "node" in the community (resp. at the boder) checks if removing it (resp. adding it) increases the quality function, if so remove it (resp. add it).
void iteration(community *com,sparse *g,qualfonc quality){
	unsigned i,node;
	for (i=0;i<com->nlist;i++){
		//size constraint
		if ((com->feat)->s > ARG.max){
			break;
		}
		node=com->list[i];
		//remove "node" from list if the node is not in the community or at the border
		if ((com->in[node]==0) && (com->bor[node]==0)){
			com->list[i--]=com->list[--com->nlist];
			com->inlist[node]=0;
			continue;
		}
		if (com->in[node]==1){
			//if node is in check if removing it increases the quality and remove it if it does.
			if (com->qual<check_rm(com,node,quality)){
				update_rm(com,g,node,quality);
			}
		}
		else{
			//if node is out check if adding it increases the quality and add it if it does.
			if (com->qual<check_add(com,node,quality)){
				update_add(com,g,node,quality);
			}
		}
	}
}

//for each "node" at the boder of the community checks if adding it increases the quality function, if so add it.
void iteration_onlyadd(community *com,sparse *g,qualfonc quality){
	unsigned i,node;
	for (i=0;i<com->nlist;i++){
		//size constraint
		if ((com->feat)->s > ARG.max){
			break;
		}
		node=com->list[i];
		//remove "node" from list if the node is not in the community or at the border
		if ((com->in[node]==0) && (com->bor[node]==0)){
			com->list[i--]=com->list[--com->nlist];
			com->inlist[node]=0;
			continue;
		}
		if (com->in[node]==0){
			//if node is out check if adding it increases the quality and add it if it does.
			if (com->qual<check_add(com,node,quality)){
				update_add(com,g,node,quality);
			}
		}
	}
}

//remove or add the node leading to the highest increase of the quality
void iteration_greedy(community *com,sparse *g,qualfonc quality){
	unsigned i,node,imax=-1;
	double quald,max=0;
	//size constraint
	if ((com->feat)->s > ARG.max){
		return;
	}
	for (i=0;i<com->nlist;i++){
		node=com->list[i];
		//remove "node" from list if the node is not in the community or at the border
		if ((com->in[node]==0) && (com->bor[node]==0)){
			com->list[i--]=com->list[--com->nlist];
			com->inlist[node]=0;
			continue;
		}
		if (com->in[node]==1){
			quald=check_rm(com,node,quality)-com->qual;
			if (quald>max){
				max=quald;
				imax=i;
			}
		}
		else{
			quald=check_add(com,node,quality)-com->qual;
			if (quald>max){
				max=quald;
				imax=i;
			}
		}
	}
	if (max>0){
		node=com->list[imax];
		if (com->in[node]==1){
			quald=check_rm(com,node,quality);
			update_rm(com,g,node,quality);
		}
		else{
			quald=check_add(com,node,quality);
			update_add(com,g,node,quality);
		}
	}
}

//add the node leading to the highest increase of the quality
void iteration_greedy_onlyadd(community *com,sparse *g,qualfonc quality){
	unsigned i,node,imax=-1;
	double quald,max=0;
	//size constraint
	if ((com->feat)->s > ARG.max){
		return;
	}
	for (i=0;i<com->nlist;i++){
		node=com->list[i];
		//remove "node" from list if the node is not in the community or at the border
		if ((com->in[node]==0) && (com->bor[node]==0)){
			com->list[i--]=com->list[--com->nlist];
			com->inlist[node]=0;
			continue;
		}
		if (com->in[node]==0){
			quald=check_add(com,node,quality)-com->qual;
			if (quald>max){
				max=quald;
				imax=i;
			}
		}
	}
	if (max>0){
		node=com->list[imax];
		quald=check_add(com,node,quality);
		update_add(com,g,node,quality);
	}
}


void optimize(sparse* g,community* com,unsigned node, qualfonc quality){
	double qual;

	inicom(com, g, node, quality);
	do {
		qual=com->qual;
		randomize(com);
		if (ARG.rm){
			if (ARG.greedy)
				iteration_greedy(com, g, quality);
			else
				iteration(com, g, quality);
		}
		else{
			if (ARG.greedy)
				iteration_greedy_onlyadd(com, g, quality);
			else
				iteration_onlyadd(com, g, quality);
		}
	} while (com->qual>qual+EPS);

}

//extract the nodes in the community
void mkcom(community* com){
	unsigned i=0,j,node;
	for (j=0;j<com->nlist;j++) {
		node=com->list[j];
		if (com->in[node]==1){
			com->nodes[i++]=node;
		}
	}
}
