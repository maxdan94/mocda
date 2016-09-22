#include "funcqual.h"

features *allocfeat(sparse *g){
	features* feat=malloc(sizeof(features));
	feat->n=g->n;
	feat->e=g->e;
	feat->lin=0;
	feat->lout=0;
	feat->s=0;
	if (ARG.tr){
		feat->tr=g->t;//TR
		feat->tr3=0;//TR
		feat->tr2=0;//TR
		feat->tr1=0;//TR
	}
	return feat;
}

double rd(features *feat){
	if (feat->lin !=0 ){
			return ((double)(feat->lin)) / ( (double)((feat->lin) + (feat->lout)) ) ;
	}
	return 0;
}

double phi(features *feat){
	if (feat->lout !=0 ){
			double v1=2*feat->lin+feat->lout;
			double v2=2*feat->e-v1;
			if (v1<v2)
				return -((double)(feat->lout)) / v1;
			return -((double)(feat->lout)) / v2;
	}
	return 0;
}

double ad(features *feat){
	if (feat->lin !=0 ){
			return ((double)(2*feat->lin)) / ( (double)((feat->s)) ) ;
	}
	return 0;
}

double ed(features *feat){
	double s=(double)(feat->s);
	return -s*(s-1.)/2.+((double)(feat->lin))-((double)(feat->lout));
}

double rdt(features *feat){//TR
	if (feat->tr3 !=0 ){
			return ((double)(feat->tr3)) / ( (double)((feat->tr3) + (feat->tr2)) ) ;
	}
	return 0;
}

//average number of triangles per nodes
double atd(features *feat){//TR
	if (feat->tr3 !=0 ){
			return ((double)(3*feat->tr3)) / ((double)(feat->s)) ;
	}
	return 0;
}

double cohesion(features *feat){//TR
	if (feat->tr3 !=0 ){
			return ((double)(6*feat->tr3*feat->tr3)) / ( (double)(((feat->tr3) + (feat->tr2))*(feat->s*(feat->s-1)*(feat->s-2))) ) ;
	}
	return 0;
}

double f1(features *feat){
	if (feat->s !=0 ){
			return (((double)(feat->lin))-((double)(feat->lout))) / ((double)(feat->s)) ;
	}
	return -feat->e;
}

double f2(features *feat){
	if (feat->lin !=0 ){
			return ((double)(feat->lin)) / pow( (double)((feat->s)),1.5 ) ;
	}
	return 0;
}

double f3(features *feat){
	if (feat->lin !=0 ){
			return ((double)(feat->lin)) / sqrt( (double)((feat->s)) ) ;
	}
	return 0;
}

double f4(features *feat){
	return rd(feat)*ad(feat) ;
}

double g1(features *feat){
	if (feat->s !=0 ){
		return (((double)(feat->tr3))-((double)(feat->tr2))) / ((double)(feat->s)) ;
	}
	return -feat->tr;
}

double g2(features *feat){
	if (feat->tr3 !=0 ){
			return ((double)(feat->tr3)) / ((double)((feat->s)*(feat->s))) ;
	}
	return 0;
}

double g3(features *feat){
	return rd(feat)*rdt(feat) ;
}

double g4(features *feat){
	return ad(feat)*atd(feat) ;
}

qualfonc choosequal(char *c){
	if (ARG.print)
		printf("Chosen quality function: ");
	if (strcmp(c,"rd")==0){
		if (ARG.print)
			printf("relative density: li / (li+lo)\n");
		return rd;
	}
	if (strcmp(c,"phi")==0){
		if (ARG.print)
			printf("conductance\n");
		return phi;
	}
	if (strcmp(c,"ad")==0){
		if (ARG.print)
			printf("average degree: 2*li / n\n");
		return ad;
	}
	if (strcmp(c,"ed")==0){
		if (ARG.print)
			printf("single edit distance\n");
		return ed;
	}
	if (strcmp(c,"rdt")==0){
		if (ARG.print)
			printf("relative density of triangles: t3 / (t3+t2)\n");
		return rdt;
	}
	if (strcmp(c,"atd")==0){
		if (ARG.print)
		printf("average triangle degree: 3*t3 / n\n");
		return atd;
	}
	if (strcmp(c,"co")==0){
		if (ARG.print)
			printf("cohesion\n");
		return cohesion;
	}
	if (strcmp(c,"f1")==0){
		if (ARG.print)
			printf("(li-lo) / n\n");
		return f1;
	}
	if (strcmp(c,"f2")==0){
		if (ARG.print)
			printf("li / n**1.5\n");
		return f2;
	}
	if (strcmp(c,"f3")==0){
		if (ARG.print)
			printf("li / n**0.5\n");
		return f3;
	}
	if (strcmp(c,"f4")==0){
		if (ARG.print)
			printf("li**2 / n*(li+lo)\n");
		return f4;
	}
	if (strcmp(c,"g1")==0){
		if (ARG.print)
			printf("(t3-t2) / n\n");
		return g1;
	}
	if (strcmp(c,"g2")==0){
		if (ARG.print)
			printf("t3 / n**2\n");
		return g2;
	}
	if (strcmp(c,"g3")==0){
		if (ARG.print)
			printf("rd * rdt\n");
		return g3;
	}
	if (strcmp(c,"g4")==0){
		if (ARG.print)
			printf("ad * atd\n");
		return g4;
	}
	printf("unknown\n");
	exit(1);
}
