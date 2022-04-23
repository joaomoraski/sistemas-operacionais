CC=gcc

P1=ex03-vetores

PROGS=$(P1)

compile: $(PROGS)

$(P1):
	$(CC) $(P1).c -o $(P1) -lrt -lm

run:
	./$(P1)

clean:
	rm -rfd $(PROGS)