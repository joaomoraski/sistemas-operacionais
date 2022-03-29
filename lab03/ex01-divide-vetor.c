//
// Created by moraski on 29/03/2022.
//
#include "stdio.h"
#include "stdlib.h"

typedef struct SearchInfo {
    int initialPosition, finalPosition, threadPosition, *array;
} SearchInfo;

SearchInfo *createSearch(int initialPosition, int finalPosition, int threadPosition, int *array) {
    SearchInfo searchInfo = (SearchInfo*) malloc(sizeof(SearchInfo));
    searchInfo.initialPosition = initialPosition;
    searchInfo.finalPosition = finalPosition;
    searchInfo.array = array;
    searchInfo.threadPosition = threadPosition;
    return searchInfo;
};

int *random_vector(int n, int max, int seed) {
    int *vector = malloc(sizeof(int) * n);
    srand(seed);
    for (int i = 0; i < n; ++i) {
        vector[i] = rand() % max;
    }
    return vector;
}


int main(int argc, char **argv) {
    if (argc > 3) return printf("Quantidade de parametros errada");
    if (argc < 3) {
        printf("Informe o numero de threads e o valor a procurar na execução do programa e o tamanho do vetor");
        return;
    }
    int threads = atoi(argv[1]), search = atoi(argv[2]), vetLen = atoi(argv[3]);
    if (threads > 15) return 0;
    int *vector = random_vector(vetLen, 100, 42);
    int divs = vetLen / threads, init = 0, final = 0;
    if (divs == 1) {
        init = 0;
        final = vetLen;
    }


    return 0;
}


