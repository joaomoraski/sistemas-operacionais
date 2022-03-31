compile:
	gcc -c sorts/quicksort.c matriz/matriz.c

run:
	gcc ex03-multithread.c matriz.o quicksort.o -o main -lpthread  && ./main 1
run2:
	gcc ex03-multithread.c matriz.o quicksort.o -o main -lpthread  && ./main 2
run4:
	gcc ex03-multithread.c matriz.o quicksort.o -o main -lpthread  && ./main 4
run8:
	gcc ex03-multithread.c matriz.o quicksort.o -o main -lpthread  && ./main 8
run16:
	gcc ex03-multithread.c matriz.o quicksort.o -o main -lpthread  && ./main 16

clean:
	rm -rfd main *.o

