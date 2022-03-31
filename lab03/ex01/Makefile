CC=gcc

P1=ex01-divide-vetor

PROGS=$(P1)

compile: $(PROGS)

$(P1):
	$(CC) $(P1).c -o $(P1) -lpthread

run:
	./$(P1) 8 9 1000

clean:
	rm $(PROGS)
