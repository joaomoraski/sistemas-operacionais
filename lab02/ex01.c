//
// Created by moraski on 22/03/2022.
//
#include "unistd.h"
#include "stdio.h"
#include "stdio.h"
int main(int argc, char *argv[]) {
    if (argc > 2) {
        return printf("Quantidade de parametros errada");
    }
    int n;
    if (argc < 2) {
        printf("Informe o nivel da arvore");
        scanf("%d", &n);
    }
    if (argc >= 2) n = atoi(argv[1]);


    int arvorePai[n];
    int nivelArvore = 0, pai, pid;

    for (int i = 0; i < n; ++i) {
        pai = getpid();
        pid = fork();
        if (!pid){
            arvorePai[nivelArvore] = pai;
            nivelArvore++;
        }
    }

    for(int i = 0; i < nivelArvore; i++) printf("PaiID %d -\t", arvorePai[i]); //imprime pai
    printf("((PID: %d))\n", getpid()); //imprime filho
    return 0;
}

