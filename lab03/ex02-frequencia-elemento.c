/*
 * Created by moraski on 29/03/2022.
 * A ideia do código é separar o vetor em 2 threads e calcular a frequencia de valores no vetor, e apos isso printar todos.
 * */
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

typedef struct ThreadInfo {
    int initialPosition, finalPosition, threadPosition, *vector;
} ThreadInfo;

// Usando uma ideia baseada na orientacao objeto e no exercicio 03 de exemplos, seria mais facil instanciar structs das threads do que criar uma por uma manualmente
ThreadInfo *createThreadStruct(int initialPosition, int finalPosition, int threadPosition, int *vector) {
    ThreadInfo *threadInfo = (ThreadInfo *) malloc(sizeof(ThreadInfo));
    threadInfo->initialPosition = initialPosition;
    threadInfo->finalPosition = finalPosition;
    threadInfo->vector = vector;
    threadInfo->threadPosition = threadPosition;
    return threadInfo;
};

int numberFreq[2][10]; // variavel global para salvar a frequencia dos numeros

int *random_vector(int n, int max, int seed) {
    int *vector = malloc(sizeof(int) * n);
    srand(seed);
    for (int i = 0; i < n; ++i) {
        vector[i] = rand() % max;
    }
    return vector;
}

void *createThread(void *param) {
    struct ThreadInfo *threadInfo = param;
    for (int i = threadInfo->initialPosition; i < threadInfo->finalPosition; ++i) {
        numberFreq[threadInfo->threadPosition][threadInfo->vector[i]]++;
    }
}

int main() {
    pthread_t vecThreads[2];
    int *vector = random_vector(1000, 10, 42), init, final;
    for (int i = 0; i < 2; ++i) {

        /*
         * Calculo do inicio e do fim do vetor para a divisao ficar correta
         * */
        init = i * (1000 / 2);
        if (i + 1 > 2) final = 1000;
        else final = ((1000 / 2) - 1) * (i + 1);

        // Criacao da struct da thread
        ThreadInfo *threadInfo = createThreadStruct(init, final, i, vector);

        pthread_create(&vecThreads[i], NULL, createThread, (void *) threadInfo);
    }

    // Espera as duas terminar para que seja feito os calculos
    pthread_join(vecThreads[0], NULL);
    pthread_join(vecThreads[1], NULL);

    // Print final das frequencia somadas
    printf("Frequencia no vetor de 1000 números: \n");
    printf("\t (Numero - Frequencia)\n");
    // Codigo esta fixo de i < 10 pq é pedido de 0 a 9 no exercicio
    for (int i = 0; i < 10; ++i) {
        printf("\t (%d - %d)\n", i, numberFreq[0][i] + numberFreq[1][i]);
    }

    return 0;
}


