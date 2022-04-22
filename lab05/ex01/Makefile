CC=gcc

P1=p1-write
P2=p2-read

PROGS=$(P1) $(P2)

compile: $(PROGS)

$(P1):
	$(CC) $(P1).c -o $(P1) -lrt

$(P2):
	$(CC) $(P2).c -o $(P2) -lrt

run1:
	./$(P1)

run2:
	./$(P2)

clean:
	rm -rfd $(PROGS)