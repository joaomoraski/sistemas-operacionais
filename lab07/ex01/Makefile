CC=gcc

P1=semaforo-leitores-escritores

PROGS=$(P1)

compile: $(PROGS)

$(P1):
	$(CC) $(P1).c -o $(P1) -lpthread

run:
	./$(P1) 3 10

clean:
	rm -rfd $(PROGS) buffer.txt