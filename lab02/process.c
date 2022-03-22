#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>


void main(int argc, char *argv[]) {
    int n;
    if (argc > 2) {
        printf("Quantidade de parametros deve ser 1");
        return;
    }
    if (argc == 1) {
        printf("Informe um número de N: ");
        scanf("%d", &n);
    } else {
        n = atoi(argv[1]);
    }
    pid_t pidPai; // guardar o pid do processo
    int status; // status dos filhos criados

    for (int i = 0; i < n; i++) {
        pidPai = fork();                       // Inicia um processo filho
        if (pidPai) {                            // Se for o pai
            pidPai = fork();                   // Inicia outro processo filho
            if (pidPai) {
                wait(&status);         // Apos criar os ramos, espera os processos filhos terminarem a execução
            }
        }
    }
    system("pstree");
}