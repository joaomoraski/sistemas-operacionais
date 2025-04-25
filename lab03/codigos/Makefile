run01:
	gcc ativ01.c -o ativ -lpthread
	./ativ 4 4

run02:
	gcc ativ02.c matriz/matriz.c -o ativ -lpthread -lm
	./ativ 1 10000 10000
	./ativ 2 10000 10000
	./ativ 4 10000 10000
	./ativ 8 10000 10000
	./ativ 16 10000 10000

clean:
	rm -rfd ativ generated_matrix.txt result.txt