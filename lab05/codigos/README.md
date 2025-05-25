# Laboratorio 05 - Sistemas operacionais.

Os códigos dessa atividade podem ser executados pelo Makefile:

```bash
make run01-pipe
make run01-fifo-receive
make run01-fifo-send
make run02
make run03
make run04-client
make run04-server
make clean
```

Todos os códigos que envolvem a execução recebendo alguma entrada para executar podem ser executados por entradas,
diretamente na execução, para mais exemplos verificar Makefile.

## Atividade 01-Pipe

Não prestei atenção e achei que era para fazer com PIPE e FIFO

Mas a execução pode ser customizada passando os valores

```bash
	gcc ativ01-pipe.c -o ativ
	./ativ "O macaco(filme) é muito estranho"
```

## Atividade 01-Fifo

A execução da atividade 01-Fifo pode ser customizada com o comando

### Para o receive
```bash
    gcc ativ01-fifo-receive.c -o ativ-receive
    ./ativ-receive
```

### Para o send
```bash
    gcc ativ01-fifo-send.c -o ativ-send
    ./ativ-send N
```

N sendo o número de mensagens.

## Atividade 02

A execução da atividade 02 não pode ser customizada, mas é possivel executar sem o Makefile

```bash
	gcc ativ02.c -o ativ
	./ativ
```

## Atividade 03

A execução da atividade 03 pode ser customizada com o comando

```bash
	gcc ativ03.c utils/utils.c -o ativ
	./ativ N M
```

N sendo a quantidade de elementos no vetor e M sendo a quantidade de processos.<br>
O resultado é salvo em result.txt

## Atividade 04

A execução da atividade 04 não pode ser customizada, mas é possivel executar sem o Makefile

### Para o client
```bash
	gcc ativ04-client.c -o ativ-client
	./ativ-client
```

### Para o server
```bash
	gcc ativ04-server.c -o ativ-server
	./ativ-server
```