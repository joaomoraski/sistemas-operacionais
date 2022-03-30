CC=gcc

P1=ex01-hierarquia-processos
P2=ex02-comando-processo
P3=ex03-divide-vetor
P4=ex04-shell

PROGS=$(P1) $(P2) $(P3) $(P4)

compile: $(PROGS)

$(P1):
	$(CC) $(P1).c -o $(P1) -lpthread
$(P2):
	$(CC) $(P2).c -o $(P2)
$(P3):
	$(CC) $(P3).c -o $(P3)
$(P4):
	$(CC) $(P4).c -o $(P4)


run1:
	./$(P1)

run2:
	./$(P2) ls
	#Deixei com valor padrao(ls), pra mudar vc teria que mudar no Makefile ou executar na mao ./nome e trocar na frente

run3:
	./$(P3) 10 20
	#Deixei com valor padrao(10 filhos pra achar o numero 20), pra mudar vc teria que mudar no Makefile ou executar na mao ./nome e trocar na frente

run4:
	./$(P4)

clean:
	rm $(PROGS)
