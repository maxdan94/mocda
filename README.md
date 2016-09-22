# README #

## Info ##
This program aims at finding all overlapping communities in a large network.

Starting from a set of nodes consisting of a single random node, the program greedily optimizes (by adding or removing one node at a time) a function quantifying the quality of a set of nodes as a community. It does this many times and output each found set of nodes if (i) its quality is high enough and (ii) it is not too similar to a previously found one and (iii) it was found several times. Default parameters are in the file param.h and can be changed manually, particularly several quality functions are implemented and can be changed.

## Compile ##

type "make" to compile the C code.

## Usage ##

type "./allcom graph.txt"

- graph.txt is the edge list of the input graph with one edge on each line and separated by spaces.
- Example for a triangle graph:  
0 1  
1 2  
2 3  

This will run the program with default parameters.
If you wish to change those, please refer to the section "Options".

## Output ##

"node_coms" contains on each line a node id followed by the ids of the communities it belongs to.

"com_nodes" contains on line i the ids of the nodes in community (i-1).

"info" contains on line i information about community (i-1):
- its size,
- its node representative,
- the number of times it was found,
- the number of in links,
- the number of out links,
- the number of triangles with 3 nodes inside,
- the number of triangles with exactly 2 nodes inside,
- the number of triangles with exactly 1 node inside,
- and its quality.

"params" contains the values of the parameters used.

## Options ##

./allcoms graph.txt [-qfonc char*] [-tr bool] [-qual float] [-sim float] [-max int] [-min int] [-times int] [-rep int] [-ini int] [-rm bool] [-order bool] [-path char*] [-print bool]

- qfonc char* : quality function choice: rd, phi, ad, ed, rdt, atd, cohesion... (cf funcqual.c).
- tr bool : //Keep track of triangles?: 0=No,1=Yes.
- qual float : minimal quality to consider a community.
- sim float : similarity threshold between two communities.
- max int : maximum size of a found communities.
- min int : minimum size of a found communities.
- times int : minimal number of times a set of nodes has to be found to be considered as a community.
- rep int : number of iterations = number of nodes times rep.
- ini int : initialization of a community, 1=a random node, 2=a random node and a random neighbor, 3=a random node and all its neighbors.
- rm bool : authorize removal
- order bool : order for the cleaning 0=natural, 1=decreasing quality
- path char* : path to the directory to output files
- print int : 0=do not print, 1=print basic info, 2=printprogress

## Initial contributors ##

Maximilien Danisch and Noe Gaumont  
Technical consultants: Jean-Loup Guillaume  
Avril 2015  
http://bit.ly/maxdan94  
maximilien.danisch@telecom-paristech.fr

