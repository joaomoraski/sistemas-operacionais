/*
 * Created by moraski on 29/03/2022.
 * O código abaixo recebe como parâmetros o número de threads, o valor a procurar e o tamanho do vetor,
 * se executado sem os parâmetros ele da erro e mostra os parâmetros necessários.
 * A ideia do código é separar o vetor em N threads e buscar um valor dentro destes vetores com cada thread buscando para sua parte do vetor.
 * */
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
            // +1 para mostrar certo a thread no log
            printf("A %d° thread encontrou o valor %d no vetor.\n", thread->threadPosition + 1, thread->key);
        }
    }
}

int main(int argc, char **argv) {
    // Ifs de garantia para os parametros estarem certos
    if (argc > 4) return printf("Quantidade de parametros errada");
    if (argc < 4) return printf("Informe o numero de threads e o valor a procurar na execução do programa e o tamanho do vetor\n");

    // Variaveis tiradas dos args para facilitar futuramente
    int nThreads = atoi(argv[1]), search = atoi(argv[2]), vetLen = atoi(argv[3]), init, final;
    // 8 threads de limite (Os testes deste algortimo foram feitos em um computador com um processamento mais baixo)
    if (nThreads > 8) return 0;

    pthread_t vecThreads[nThreads];
    int *vector = random_vector(vetLen, 10, 42);

    /*
     * Calculo do inicio e do fim do vetor para a divisao ficar correta
     * */
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


