/*
    * Funcionalidade: Programa para resolver o problema de Leitores e Escritores utilizando Semáforo.
    * Alunos: Caio Luiz dos Santos, Flávio Augusto Bernaski da Silva, João Vitor Moraski Lunkes
    * Data: 12/05/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex, mutexLeitor; //Mutex
int leitorLock = 1; //Fecha a entrada de leitores
int leitores = 0; //Número de leitores
int bloqueado = 0; //Variável para mostrar qual foi o leitor bloqueado sem que fique muito lixo na tela

//Função leitor para ler o buffer, recebe de parâmetro a thread utilizada
void leitor(void *threadId) {
    int threadIdVar = *((int*) threadId); //Define o ID do leitor de acordo com a thread
    while(1) {
        //Verifica se o leitor está bloqueado
        if (!leitorLock) {
            bloqueado = 0; //Reseta a variável bloqueado para controle de bloqueios futuros

            //Usa semáforo para utilizar a variável compartilhada
            sem_wait(&mutexLeitor);
            leitores++;
            sem_post(&mutexLeitor);

            //Abre o buffer.txt, lê o que tem nele e escreve na varíavel buffer
            int buffer;
            FILE *arq = fopen("buffer.txt", "r");
            fscanf(arq, "%d", &buffer);
            fclose(arq);

            printf("Leitor ID %d leu o que o Escritor ID %d escreveu.\nTotal de leitores: %d\n", threadIdVar, buffer, leitores);

            sleep(1); //Simulação de processamento

            //Usa semáforo para utilizar a variável compartilhada
            sem_wait(&mutexLeitor);
            leitores--;
            sem_post(&mutexLeitor);
            //Caso esteja bloqueado
        } else {
            //Verifica se já printou o leitor bloqueado, caso não tenha printado, printa e aumenta o valor de bloqueado, que são as tentativas falhas
            if (bloqueado == 0) {
                printf("Leitor ID %d bloqueado pelo bloqueador de leitura.\n", threadIdVar);
                bloqueado++;
            }
        }
    }
}

void escritor(void *threadId) {
    int threadIdVar = *((int*) threadId); //Define o ID do leitor de acordo com a thread
    while(1) {
        //Usa semáforo para bloquear que os leitores leiam o buffer
        sem_wait(&mutex);
        leitorLock = 1;

        //Abre o buffer.txt e escreve nele
        FILE *arq = fopen("buffer.txt", "w");
        fprintf(arq, "%d", threadIdVar);
        fclose(arq);

        printf("Escritor ID %d escreveu.\n", threadIdVar);

        sleep(1); //Simulação de processamento

        //desbloqueia os leitores
        leitorLock = 0;

        //Espera entre os escritores
        sleep(2);
        sem_post(&mutex);
        sleep(3);
    }
}

int main(int argc, char *argv[]) {
    //Verificacao das informacoes necessarias
    if (argc < 3) return printf("./$ <Numero de Threads> <Proporcao>\n");
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