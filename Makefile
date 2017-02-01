CC=gcc
CFLAGS=-O9 -lm
LDFLAGS=
EXEC=main

all: $(EXEC)

main: graph.o funcqual.o triangle.o optim.o clean.o param.o main.o
	$(CC) -o mocda graph.o funcqual.o triangle.o optim.o clean.o param.o main.o $(CFLAGS)

graph.o: graph.c
	$(CC) -o graph.o -c graph.c $(CFLAGS)

funcqual.o: funcqual.c
	$(CC) -o funcqual.o -c funcqual.c $(CFLAGS)

triangle.o:  triangle.c
	$(CC) -o triangle.o -c triangle.c $(CFLAGS)

optim.o: optim.c
	$(CC) -o optim.o -c optim.c $(CFLAGS)

clean.o: clean.c
	$(CC) -o clean.o -c clean.c $(CFLAGS)

param.o: param.c
	$(CC) -o param.o -c param.c $(CFLAGS)

main.o: main.c
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm *.o mocda

cleanall:
	rm *.o mocda info params node_coms com_nodes
