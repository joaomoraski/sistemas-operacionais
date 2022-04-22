#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

// Inclua em todos os arquivos de código fonte um cabeçalho com a funcionalidade, autor(es) e data.


int getNumeroEspaco(char palavra[40]) {
    int contEspaco = 0;
    for (int i = 0; i < strlen(palavra); ++i) {
        if (palavra[i] == ' ') {
            contEspaco++;
        }
    }
    return contEspaco;
}

int getVogais(char palavra[40]) {
    char vogais[10] = {'A', 'E', 'I', 'O', 'U', 'a', 'e', 'i', 'o', 'u'};
    int contVogais = 0;
    for (int i = 0; i < strlen(palavra); ++i) {
        for (int j = 0; j < 10; ++j) {
            if (palavra[i] == vogais[j]) {
                contVogais++;
            }
        }
    }
    return contVogais;
}

int getConsoantes(char palavra[40]) {
    return (strlen(palavra) - getVogais(palavra) - getNumeroEspaco(palavra));
}

/* Programa principal */
int main(void) {
    pid_t pid;
    int mypipe[2];
    char buffer[40];

    /* Criar o pipe. */
    if (pipe(mypipe)) {
        fprintf(stderr, "Falha ao criar o Pipe.\n");
        return EXIT_FAILURE;
    }

    /* Criar o processo filho. */
    pid = fork();
    if (pid < (pid_t) 0) {
        /* pid negativo, falha no fork. */
        fprintf(stderr, "Falha ao executar o Fork.\n");
        return EXIT_FAILURE;
    } else if (pid == (pid_t) 0) {
        /* No processo filho. */
        close(mypipe[1]);
        read(mypipe[0], buffer, sizeof(buffer));
        printf("String enviada: %s\n", buffer);
        printf("Tamanho da string: %lu\n", strlen(buffer));
        printf("Número de consoantes: %d\n", getConsoantes(buffer));
        printf("Número de vogais: %d\n", getVogais(buffer));
        printf("Número de espação: %d\n", getNumeroEspaco(buffer));
        fflush(stdout);
        return EXIT_SUCCESS;
    } else {
        /* Processo pai. */
        close(mypipe[0]);
        printf("PAI: Digite algo para enviar: ");
        scanf("%40[^\n]", buffer);
        write(mypipe[1], buffer, sizeof(buffer));

        wait(NULL);
        return EXIT_SUCCESS;
    }
}
