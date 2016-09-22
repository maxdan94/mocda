#include "param.h"
#include <getopt.h>
#include <stdlib.h>

//global variable for option parsing:
char *optarg;
int optind, optopt;
//arguments by default:
arguments ARG={NULL,QFONC,TR,QUAL,SIM,MAX,MIN,TIMES,REP,INI,RM,GREEDY,ORDER,PATH,PRINT};

//parsing the options
int read_param(int argc, char **argv, arguments* arg){
	char c;
	unsigned index=0;
  struct option long_options[] = {
    {"qfonc",required_argument,0,'a'},
    {"tr",required_argument,0,'b'},
    {"qual",required_argument, 0,'c'},
    {"sim",required_argument, 0,'d'},
    {"max",required_argument, 0,'e'},
    {"min",required_argument, 0,'f'},
    {"times",required_argument, 0,'g'},
    {"rep",required_argument, 0,'h'},
		{"ini",required_argument, 0,'i'},
		{"rm",required_argument, 0,'j'},
		{"greedy",required_argument, 0,'k'},
		{"order",required_argument, 0,'l'},
		{"path",required_argument, 0,'m'},
		{"print",required_argument, 0,'n'},
    {0, 0, 0, 0}
  };
	while (1){
    c = getopt_long_only (argc, argv, "a:b:c:d:e:f:g:h:i:j:l:m:n:",long_options, &index);
    if (c==-1){
      break;
    }
		switch (c){
			case 'a':
				arg->qfonc=optarg;
				break;
      case 'b':
				arg->tr = atoi(optarg);
				break;
			case 'c':
				arg->qual = atof(optarg);
				break;
			case 'd':
				arg->sim = atof(optarg);
				break;
			case 'e':
				arg->max = atoi(optarg);
				break;
      case 'f':
				arg->min = atoi(optarg);
				break;
			case 'g':
				arg->times = atoi(optarg);
				break;
			case 'h':
				arg->rep = atoi(optarg);
				break;
			case 'i':
				arg->ini = atoi(optarg);
				break;
			case 'j':
				arg->rm = atoi(optarg);
				break;
			case 'k':
				arg->greedy = atoi(optarg);
				break;
			case 'l':
				arg->order = atoi(optarg);
				break;
			case 'm':
				arg->path = optarg;
				break;
			case 'n':
				arg->print = atoi(optarg);
				break;
			case '?':
				return 1;
			default:
				return 1;
		}
	}
	if (optind == argc){
		fprintf (stderr, "A path to the graph is needed.\n");
		return 1;
	}
	if(optind < argc-1){
		fprintf (stderr, "Only a path to the graph is needed.\nYou provided %d arguments:\n", argc-optind);
		for (index = optind; index < argc; index++)
			printf ("\t-%s\n", argv[index]);
		return 1;
	}
	arg->graph = argv[optind];
	return 0;
}

void printparams(char* params){
	FILE* file=fopen(params,"w");
	fprintf(file,"qfonc %s\n",ARG.qfonc);
	fprintf(file,"tr %u\n",ARG.tr);
	fprintf(file,"qual %le\n",ARG.qual);
	fprintf(file,"sim %le\n",ARG.sim);
	fprintf(file,"max %u\n",ARG.max);
	fprintf(file,"min %u\n",ARG.min);
	fprintf(file,"times %u\n",ARG.times);
	fprintf(file,"rep %u\n",ARG.rep);
	fprintf(file,"ini %u\n",ARG.ini);
	fprintf(file,"rm %u\n",ARG.rm);
	fprintf(file,"greedy %u\n",ARG.greedy);
	fprintf(file,"order %u\n",ARG.order);
	fclose(file);
}
