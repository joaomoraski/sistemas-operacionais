//
// Created by moraski on 20/03/2022.
//
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "sys/wait.h"

int main(int argc, char **argv) {
    int status;
    char path[300] = "/bin/";
    if (argc > 2 || argc == 1) {             // o primeiro parametro é o nome do arquivo
        printf("Digite apenas a quantidade de parametros permitida(1)\n");
        return 0;
    }
    pid_t pid = fork();                     // cria filho
    if (!pid) {                             // filho
        strcat(path, argv[1]);
        char *cmd[] = {argv[1], (char *) 0};
        execv(path, cmd);
    } else {                                // pai
        wait(&status);              // espera o filho executar
    }
    return 0;
}