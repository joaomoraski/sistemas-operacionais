/*
* 1. a) Faça um programa que lê strings digitadas pelo usuário e envia para outro programa que recebe
essas strings e exibe na tela: a string, o tamanho, o número de consoantes, o número de vogais e o
número de espaços
* Autor: João Moraski | RA: 1904000
* Data de criação: 23/05/2025
*/

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 101
#define SERVER_FIFO "/tmp/serverfifo"

int main(int argc, char* argv[]) {
    char entry[MAX];
    int wordsCount = 0;
    int i = 0;

    if (argc < 2) {
        printf("Informe a quantidade de mensagens: ");
        scanf("%d", &wordsCount);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    } else {
        wordsCount = atoi(argv[1]);
    }

    if (mkfifo(SERVER_FIFO, 0664) == -1 && errno != EEXIST) {
        perror("Error to create FIFO");
        return EXIT_FAILURE;
    }

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("Open error on fifo");
        return EXIT_FAILURE;
    }

    while (i < wordsCount) {
        printf("Informe a mensagem (100 caracteres): ");
        if (fgets(entry, MAX, stdin) == NULL) {
            perror("Error reading message");
            break;
        }
        entry[strcspn(entry, "\n")] = '\0';

        if (strlen(entry) > 0) {
            ssize_t bytes_written = write(fd, entry, strlen(entry));
            if (bytes_written == -1) {
                perror("Error writing to FIFO");
                break;
            }
        }
        sleep(1);
        i++;
    }

    if (close(fd) == -1) {
        perror("close error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}