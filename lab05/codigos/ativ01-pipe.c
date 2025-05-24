/*
* 1. a) Faça um programa que lê strings digitadas pelo usuário e envia para outro programa que recebe
essas strings e exibe na tela: a string, o tamanho, o número de consoantes, o número de vogais e o
número de espaços, usando fifo
* Autor: João Moraski | RA: 1904000
* Data de criação: 23/05/2025
*/

#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 101  // 100 + \0

void read_entry(int argc, char* argv[], char* entry);
void make_logic(const char* buffer);

int main(int argc, char* argv[]) {
    pid_t pid;
    int pipes[2];
    char buffer[MAX];
    char entry[MAX];

    read_entry(argc, argv, entry);

    if (pipe(pipes)) {
        fprintf(stderr, "Falha ao criar\n");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Falha ao criar fork\n");
        return EXIT_FAILURE;
    }

    // pai
    if (pid == 0) {
        // filho
        close(pipes[1]); // fecha escrita
        read(pipes[0], buffer, sizeof(buffer));
        // printf("[DEBUG] Filho leu %ld bytes\n", n);
        make_logic(buffer);
        close(pipes[0]);
        exit(EXIT_SUCCESS);
    }
    else {
        close(pipes[0]); // fecha leitura
        strcpy(buffer, entry); // copiar o que o usuário digitou para o buffer
        // printf("[DEBUG] Pai vai escrever: '%s'\n", buffer);
        write(pipes[1], buffer, strlen(buffer) + 1);
        close(pipes[1]);
        wait(NULL);
    }

    // Finalização
    // fflush(stdout);
    return EXIT_SUCCESS;
}


void read_entry(int argc, char* argv[], char* entry) {
    if (argc < 2) {
        printf("Informe a mensagem(100 caracteres): ");
        scanf("%100[^\n]", entry);
    }
    else {
        strncpy(entry, argv[1], MAX);
    }
}

void make_logic(const char* buffer) {
    int vogais = 0, consoantes = 0, espacos = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        char c = tolower(buffer[i]);
        if (strchr("aeiou", c)) vogais++;
        else if (c >= 'a' && c <= 'z') consoantes++;
        else if (c == ' ') espacos++;
    }
    printf("Frase informada: %s\n", buffer);
    printf("Tamanho: %lu\n", strlen(buffer));
    printf("Número de vogais: %d\n", vogais);
    printf("Número de consoantes: %d\n", consoantes);
    printf("Número de espaços: %d\n", espacos);
}
