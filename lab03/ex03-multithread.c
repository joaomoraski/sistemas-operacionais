/*
 * Created by moraski on 30/03/2022.
 * A ideia do código é separar a quantidade de threads passada para o programa e dividir em dois para fazer os exercicios A e B
 * a) A mediana de cada linha de uma matriz MxN e devolva o resultado em um vetor de tamanho M
 * B) A media aritmetica de cada coluna de uma matriz MxN e devolva o resultado em um vetor de tamanho N
 * */
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "/matriz/matriz.h"

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

int main() {
    int row,col;
    create_matrix(1000,1000,2000);

    // le a matriz gerada na linha anteior
    int** matriz = read_matrix_from_file("matriz_entrada.in", &row, &col);

    return 0;
}
