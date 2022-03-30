/*
 * Created by moraski on 30/03/2022.
 * A ideia do código é separar a quantidade de threads passada para o programa e dividir em dois para fazer os exercicios A e B
 * a) A mediana de cada linha de uma matriz MxN e devolva o resultado em um vetor de tamanho M
 * B) A media aritmetica de cada coluna de uma matriz MxN e devolva o resultado em um vetor de tamanho N
 * */
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "matriz/matriz.h"
#include "sorts/quicksort.h"

#define SIZE 1000

typedef struct ThreadInfo {
    int initialPosition, finalPosition, threadPosition, **matriz;
} ThreadInfo;

// Usando uma ideia baseada na orientacao objeto e no exercicio 03 de exemplos, seria mais facil instanciar structs das threads do que criar uma por uma manualmente
ThreadInfo *createThreadStruct(int initialPosition, int finalPosition, int threadPosition, int **matriz) {
    ThreadInfo *threadInfo = (ThreadInfo *) malloc(sizeof(ThreadInfo));
    threadInfo->matriz = matriz;
    threadInfo->initialPosition = initialPosition;
    threadInfo->finalPosition = finalPosition;
    threadInfo->threadPosition = threadPosition;
    return threadInfo;
}

int medianaVec[SIZE];
int mediaAritmeticaVec[SIZE];

void *mediana(void *param) {
    struct ThreadInfo *threadInfo = param;
    for (int i = threadInfo->initialPosition; i < threadInfo->finalPosition; ++i) {
        QuickSort(threadInfo->matriz[i], threadInfo->matriz[i][0], threadInfo->matriz[i][SIZE]);
        if (SIZE / 2 == 0) {
            medianaVec[i] = threadInfo->matriz[i][SIZE/2] + threadInfo->matriz[i][(SIZE/2)+1];
        } else{
            medianaVec[i] = threadInfo->matriz[i][SIZE/2];
        }
    }
}

void *mediaAritmetica(void *param) {
    struct ThreadInfo *threadInfo = param;
    for (int i = threadInfo->initialPosition; i < threadInfo->finalPosition; ++i) {
        int soma = 0;
        for (int j = 0; j < SIZE; ++j) {
            soma += threadInfo->matriz[i][j];
            mediaAritmeticaVec[i] = soma/SIZE;
        }
    }
}

int main(int argc, char **argv) {
    int row = SIZE, col = SIZE, nThreads = atoi(argv[1]) / 2, init, final;
    pthread_t vecThreadsA[nThreads];
    pthread_t vecThreadsB[nThreads];
    // le a matriz gerada na linha anteior
    int **matriz = create_matrix(row, col);
    for (int i = 0; i < nThreads; ++i) {
        /*
         * Calculo do inicio e do fim do vetor para a divisao ficar correta
         * */
        init = i * (SIZE / nThreads);
        if (i + 1 > nThreads) final = SIZE;
        else final = ((SIZE / nThreads) - 1) * (i + 1);

        // Criacao da struct da thread
        ThreadInfo *threadInfo = createThreadStruct(init, final, i, matriz);

        pthread_create(&vecThreadsA[i], NULL, mediana, (void *) threadInfo);
    }

    return 0;
}
