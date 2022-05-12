#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

void leitor(void *threadId);
void escritor(void *threadId);

sem_t mutex, mutexLeitor;
int leitorLock = 1, leitores = 0, bloqueado = 0;

int main(int argc, char *argv[]) {
    //Verificacao das informacoes necessarias
    if (argc < 3) return printf("./$ <N_Threads> <Proporcao>\n");
    int nThread = atoi(argv[1]);
    int chance = atoi(argv[2]);
    if (nThread < 2) nThread = 2;

    //Inicializador e identificador das Threads
    int id[nThread];
    pthread_t thread[nThread];
    for (int i = 0; i < nThread; ++i) id[i] = i;

    //Inicializador dos Semaforos
    sem_init(&mutex, 0, 1);
    sem_init(&mutexLeitor, 0, 1);

    //Criacao das Threads
    srand(time(NULL));
    pthread_create(&thread[0], NULL, (void*) &escritor, (void*) &id[0]);
    for (int i = 1; i < nThread; ++i) {
        int tmp = rand() % chance;
        if (tmp == 0) pthread_create(&thread[i], NULL, (void*) &escritor, (void*) &id[i]);
        else pthread_create(&thread[i], NULL, (void*) &leitor, (void*) &id[i]);
    }

    //Sincronizar
    for (int i=0; i < nThread; i++) {
        pthread_join(thread[i], NULL);
    }
    sem_destroy(&mutexLeitor);
    sem_destroy(&mutex);
    pthread_exit(NULL);
    return 1;
}

void leitor(void *threadId) {
    int threadIdVar = *((int*) threadId);
    while(1) {
        if (!leitorLock) {
            bloqueado = 0;
            sem_wait(&mutexLeitor);
            leitores++;
            sem_post(&mutexLeitor);

            int buffer;
            FILE *arq = fopen("arquivo.txt", "r");
            fscanf(arq, "%d", &buffer);
            fclose(arq);

            printf("Leitor ID %d leu o que o Escritor ID %d escreveu.\nTotal de leitores: %d\n", threadIdVar, buffer, leitores);

            sleep(1);

            sem_wait(&mutexLeitor);
            leitores--;
            sem_post(&mutexLeitor);
        } else {
            if (bloqueado == 0) {
                printf("Leitor ID %d bloqueado pelo bloqueador de leitura.\n", threadIdVar);
                bloqueado++;
            }
        }
    }
}

void escritor(void *threadId) {
    int threadIdVar = *((int*) threadId);
    while(1) {
        sem_wait(&mutex);
        leitorLock = 1;

        FILE *arq = fopen("arquivo.txt", "w");
        fprintf(arq, "%d", threadIdVar);
        fclose(arq);

        printf("Escritor ID %d escreveu.\n", threadIdVar);

        sleep(1);

        leitorLock = 0;
        sleep(2);
        sem_post(&mutex);
        sleep(3);
    }
}