//
// Created by moraski on 29/03/2022.
//
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

typedef struct SearchInfo {
    int initialPosition, finalPosition, threadPosition, *vector, key;
} SearchInfo;

// Usando uma ideia baseada na orientacao objeto e no exercicio 03 de exemplos, seria mais facil instanciar structs das threads do que criar uma por uma manualmente
SearchInfo *createSearch(int initialPosition, int finalPosition, int threadPosition, int *vector, int key) {
    SearchInfo *searchInfo = (SearchInfo *) malloc(sizeof(SearchInfo));
    searchInfo->initialPosition = initialPosition;
    searchInfo->finalPosition = finalPosition;
    searchInfo->vector = vector;
    searchInfo->key = key;
    searchInfo->threadPosition = threadPosition;
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

void *createThread(void *param) {
    struct SearchInfo *thread = param;
    for (int i = thread->initialPosition; i < thread->finalPosition; ++i) {
        if (thread->vector[i] == thread->key) {
            // +1 para mostrar certo no log
            printf("A %d° thread encontrou o valor %d no vetor.\n", thread->threadPosition + 1, thread->key);
        }
    }
}

int main(int argc, char **argv) {
    if (argc > 4) return printf("Quantidade de parametros errada");
    if (argc < 4) {
        printf("Informe o numero de threads e o valor a procurar na execução do programa e o tamanho do vetor\n");
        return 0;
    }
    int nThreads = atoi(argv[1]), search = atoi(argv[2]), vetLen = atoi(argv[3]), init, final;
    if (nThreads > 8) return 0;

    pthread_t vecThreads[nThreads];
    int *vector = random_vector(vetLen, 10, 42);

    for (int i = 0; i < nThreads; ++i) {
        init = i * (vetLen / nThreads);
        if (i + 1 > nThreads) final = vetLen;
        else final = ((vetLen / nThreads) - 1) * (i + 1);

        SearchInfo *searchInfo = createSearch(init, final, i, vector, search);

        pthread_create(&vecThreads[i], NULL, createThread, (void *) searchInfo);
    }

    pthread_exit(0);
    return 0;
}


