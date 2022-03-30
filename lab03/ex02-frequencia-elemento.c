//
// Created by moraski on 29/03/2022.
//
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

typedef struct SearchInfo {
    int initialPosition, finalPosition, threadPosition, *vector;
} SearchInfo;

// Usando uma ideia baseada na orientacao objeto e no exercicio 03 de exemplos, seria mais facil instanciar structs das threads do que criar uma por uma manualmente
SearchInfo *createSearch(int initialPosition, int finalPosition, int threadPosition, int *vector) {
    SearchInfo *searchInfo = (SearchInfo *) malloc(sizeof(SearchInfo));
    searchInfo->initialPosition = initialPosition;
    searchInfo->finalPosition = finalPosition;
    searchInfo->vector = vector;
    searchInfo->threadPosition = threadPosition;
    return searchInfo;
};

int numberFreq[2][10];

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
        numberFreq[thread->threadPosition][thread->vector[i]]++;
    }
}

int main() {
    pthread_t vecThreads[2];
    int *vector = random_vector(1000, 10, 42), init, final;
    for (int i = 0; i < 2; ++i) {
        init = i * (1000 / 2);
        if (i + 1 > 2) final = 1000;
        else final = ((1000 / 2) - 1) * (i + 1);

        SearchInfo *searchInfo = createSearch(init, final, i, vector);

        pthread_create(&vecThreads[i], NULL, createThread, (void *) searchInfo);
    }

    pthread_join(vecThreads[0], NULL);
    pthread_join(vecThreads[1], NULL);
    // Espera as duas terminar para que seja feito os calculos
    printf("Frequencia no vetor de 1000 números: \n");
    printf("\t (Numero - Frequencia)\n");
    for (int i = 0; i < 10; ++i) {
        printf("\t (%d - %d)\n", i, numberFreq[0][i] + numberFreq[1][i]);
    }

    return 0;
}


