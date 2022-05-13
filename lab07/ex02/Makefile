CC=gcc

P1=main
PROGS=$(P1)

compile: $(PROGS)

$(P1):
	$(CC) -g *.c -o $(P1) -lpthread

run:
	./$(P1) 10

clean:
	rm -rfd $(PROGS)