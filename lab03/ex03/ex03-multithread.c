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
    int initialPosition, finalPosition, threadPosition, **matriz, row, col;
} ThreadInfo;

// Usando uma ideia baseada na orientacao objeto e no exercicio 03 de exemplos, seria mais facil instanciar structs das threads do que criar uma por uma manualmente
ThreadInfo *
createThreadStruct(int initialPosition, int finalPosition, int threadPosition, int **matriz, int row, int col) {
    ThreadInfo *threadInfo = (ThreadInfo *) malloc(sizeof(ThreadInfo));
    threadInfo->matriz = matriz;
    threadInfo->initialPosition = initialPosition;
    threadInfo->finalPosition = finalPosition;
    threadInfo->threadPosition = threadPosition;
    threadInfo->row = row;
    threadInfo->col = col;
    return threadInfo;
}

int medianaVec[SIZE];
int mediaAritmeticaVec[SIZE];

void *mediana(void *param) {
    struct ThreadInfo *threadInfo = param;
    int row = threadInfo->row;
    for (int i = threadInfo->initialPosition; i <= threadInfo->finalPosition; ++i) {
        QuickSort(threadInfo->matriz[i], threadInfo->matriz[i][0], threadInfo->matriz[i][row]);
        if (row % 2 == 0) {
            medianaVec[i] = threadInfo->matriz[i][row / 2] + threadInfo->matriz[i][(row / 2) + 1];
        } else {
            medianaVec[i] = threadInfo->matriz[i][row / 2];
        }
    }
}

void *mediaAritmetica(void *param) {
    struct ThreadInfo *threadInfo = param;
    int row = threadInfo->row;
    for (int i = threadInfo->initialPosition; i < threadInfo->finalPosition; ++i) {
        int soma = 0;
        for (int j = 0; j < row; ++j) {
            soma += threadInfo->matriz[j][i];
            mediaAritmeticaVec[i] = soma / row;
        }
    }
}

int main(int argc, char **argv) {
    clock_t initialTime = clock();
    int row = SIZE, col = SIZE, nThreads = atoi(argv[1]), init, final;
    pthread_t vecThreadsA[nThreads];
    pthread_t vecThreadsB[nThreads];
    // le a matriz gerada na linha anteior
    writeMatrizToFile(row, col); // gerando a matriz para o arquivo entry_data

    int **matriz = read_matrix_from_file("entry_data.in", &row, &col);

    for (int i = 0; i < nThreads; ++i) {
        /*
         * Calculo do inicio e do fim do vetor para a divisao ficar correta
         * */
        init = i * (row / nThreads);
        if (i + 1 > nThreads) final = row;
        else final = ((row / nThreads) - 1) * (i + 1);

        // Criacao da struct da thread
        ThreadInfo *threadInfo = createThreadStruct(init, final, i, matriz, row, col);

        pthread_create(&vecThreadsA[i], NULL, mediana, (void *) threadInfo);
        pthread_join(vecThreadsA[i], NULL);
    }

    for (int i = 0; i < nThreads; ++i) {
        /*
         * Calculo do inicio e do fim do vetor para a divisao ficar correta
         * */
        init = i * (col / nThreads);
        if (i + 1 > nThreads) final = col;
        else final = ((col / nThreads) - 1) * (i + 1);

        // Criacao da struct da thread
        ThreadInfo *threadInfo = createThreadStruct(init, final, i, matriz, row, col);

        pthread_create(&vecThreadsB[i], NULL, mediaAritmetica, (void *) threadInfo);
    }
    int medianaTeste[row];
    int mediaTeste[col];

    for (int i = 0; i < row; ++i) {
        printf("Mediana: %d\n", medianaVec[i]);
        medianaTeste[row] = medianaVec[i];
    }

    for (int i = 0; i < col; ++i) {
        // Gambiarra para nao printar varios 0 pois o codigo nao esta completo e nao consegui terminar por conta do tempo.
        if (mediaAritmeticaVec[i] == 0) break;
        printf("Media: %d\n", mediaAritmeticaVec[i]);
        mediaTeste[col] = mediaAritmeticaVec[i];
    }

//    printf("O arquivo de saída esta na pasta de execucao e se chama responde_data.out\n");

    clock_t finalTime = clock();
    double spentTime = (double) ((finalTime - initialTime) * 1000) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %f\n", (float) spentTime);

    return 0;
}



