/*
* Faça um programa com N threads que localiza um valor em um vetor de inteiros. O espaço de busca no
vetor deve ser distribuído para as N threads.
* Autor: João Moraski | RA: 1904000
* Data de criação: 22/04/2025
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define vMax 100

typedef struct {
    int start;
    int end;
    int number;
    int target;
    int* vector;
} ThreadValues;

// Gera um vetor aleatório de tamanho vMax(100)
void generateRandomVector(int* vector, int target) {
    srand(time(NULL));
    for (int i = 0; i < vMax; i++) {
        vector[i] = rand() % 100 + 1;
    }
}

void* searchInVector(void* arg) {
    // Fazer o cast para o tipo correto
    ThreadValues* threadValues = (ThreadValues*)arg;
    // Faz a busca do valor no seu específico range
    for (int i = threadValues->start; i < threadValues->end; i++) {
        if (threadValues->vector[i] == threadValues->target) {
            printf("O elemento %d foi encontrado pela thread de número: %d na posição %d\n", threadValues->vector[i],
                   threadValues->number, i);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) return printf("Informe o número de threads e o número a ser buscado\n");

    // Pega os valores da execução do arquivo
    int numThreads = atoi(argv[1]);
    int target = atoi(argv[2]);
    int vector[vMax] = {}; // Inicializa o vetor
    pthread_t threads[numThreads];
    ThreadValues* threadsData[numThreads];

    generateRandomVector(vector, target);

    int rangePerThread; // Define as variaveis de range

    rangePerThread = vMax / numThreads;

    // Faz o cálculo e divisão de threads e ranges
    for (int i = 0; i < numThreads; i++) {
        // Start seria de onde vai o i * o range para cada thread, no caso 0 começa em zero, em 1, por exemplo, para 4 threads, iria para 25.
        int start = i * rangePerThread;
        // If ternario para definir se o final poderia estourar o limite ou não
        // Calculo do end é similar ao do start, porem com i + 1
        int end = (i + 1) * rangePerThread > vMax ? vMax : (i + 1) * rangePerThread;

        // Passa os valores para a struct
        ThreadValues* threadValues = malloc(sizeof(ThreadValues));
        threadValues->start = start;
        threadValues->end = end;
        threadValues->number = i;
        threadValues->vector = vector;
        threadValues->target = target;
        threadsData[i] = threadValues;

        // Começa a criar as threads para fazer a divisao
        int thread = pthread_create(&threads[i], NULL, searchInVector, (void*)threadValues);
    }

    // Espera pela resposta dos filhos
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Limpa a memoria para evitar vazamento(nao iria ocorrer aq, mas boas praticas ne)
    for (int i = 0; i < numThreads; i++) {
        free(threadsData[i]);
    }
}
