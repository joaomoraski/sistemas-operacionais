//
// Created by moraski on 22/03/2022.
//
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "sys/wait.h"

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

    pid_t pid;
    if (pid) {
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < pow(2, i); ++j) {
                pid = fork();
            }
        }
    } else {
        pid = fork();
    }

    system("ps f");
    return 0;
}

