CC=gcc

P1=ex01-divide-vetor
P2=ex02-frequencia-elemento
P3=ex03-multithread

PROGS=$(P1) $(P2) $(P3)

compile: $(PROGS)

$(P1):
	$(CC) $(P1).c -o $(P1) -lm
$(P2):
	$(CC) $(P2).c -o $(P2)
$(P3):
	$(CC) $(P3).c -o $(P3)


run1:
	./$(P1)

run2:
	./$(P2)

run3:
	./$(P3)

clean:
	rm $(PROGS)
