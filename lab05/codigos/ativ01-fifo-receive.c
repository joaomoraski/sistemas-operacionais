/*
* 1. a) Faça um programa que lê strings digitadas pelo usuário e envia para outro programa que recebe
essas strings e exibe na tela: a string, o tamanho, o número de consoantes, o número de vogais e o
número de espaços
* Autor: João Moraski | RA: 1904000
* Data de criação: 23/05/2025
*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>  // mkfifo
#include <fcntl.h>     // open
#include <unistd.h>    // read, write, close

#define MAX 101  // 100 + \0
#define SERVER_FIFO "/tmp/serverfifo"

void make_logic(const char* buffer);

int main(int argc, char* argv[]) {
    char buffer[MAX];
    int num_bytes_read; // descritor

    if (mkfifo(SERVER_FIFO, 0664) == -1 && errno != EEXIST) {
        perror("Error to create FIFO\n");
        return EXIT_FAILURE;
    }

    int fd = open(SERVER_FIFO, O_RDONLY);
    if (fd == -1) {
        perror("Open error on fifo");
        return EXIT_FAILURE;
    }

    while (1) {
        memset(buffer, '\0', sizeof(buffer));
        num_bytes_read = read(fd, buffer, sizeof(buffer));
        switch (num_bytes_read) {
        case -1:
            perror("Error to read fifo");
            break;
        case 0:
            printf("None data, reopening fifo\n");
            close(fd);
            fd = open(SERVER_FIFO, O_RDONLY);
            break;
        default:
            printf("\nReceived data with %d bytes\n", num_bytes_read);
            make_logic(buffer);
        }
    }

    if (close(fd) == -1) {
        perror("close error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}