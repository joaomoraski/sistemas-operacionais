
/*
    * Funcionalidade: Programa para resolver o problema do Barbeiro Dorminhoco
    * Alunos: Caio Luiz dos Santos, Flávio Augusto Bernaski da Silva, João Vitor Moraski Lunkes
    * Data: 12/05/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "resource_monitor.h"

/* --- funcoes --- */
void *barbeiro();
void *cliente(void *thread_id);

/* --- main --- */
int main(int argc, char *argv[]) {
    if (argc < 2) return printf("./$ <num_de_clientes>\n");
    int numClientes = atoi(argv[1]);

    printf("numClientes: %d \n", numClientes);
    // Identificadores para as Threads. (1 barbeiro + numClientes)
    int id[(1 + numClientes)];

    for (int i = 0; i < (1 + numClientes); i++) id[i] = i;

    pthread_t barbeirod;
    pthread_t clientes[numClientes];

    initMonitor();

    //criar thread barbeiro
    pthread_create(&barbeirod, NULL, barbeiro, NULL);

    //criar threads clientes
    for (int i = 0; i < numClientes; i++){
        pthread_create(&clientes[i], NULL, cliente, (void *) &id[i + 1]);
        pthread_join(clientes[i], NULL);
    }

    // Wait for the threads to finish
    pthread_join(barbeirod, NULL);

    destroyMonitor();

    printf("Programa finalizado.\n");
    pthread_exit(0);
}


/* --- funcoes ---*/
void *cliente(void *thread_id) {
    int id = *((int *) thread_id);

    //seed do rand()
    srand(id);
    int tempo;

    while (1) {
        // dormir por um tempo aleatorio
        tempo = (rand() % 10);
        sleep(tempo);

        // cliente chegou na barbearia
        int status = put(id);
        if (status == 0) printf("CLIENTE %d: Chegou para cortar cabelo.\n", id);
        fflush(stdout);
    }
}

void *barbeiro() {
    int tipo = -1;
    while (tipo != 1) {    //parar o loop apos cortar todos os cabelos

        //Barbeiro espera o sinal
        tipo = take();
        printf("BARBEIRO: cortou cabelo do cliente.\n");
        fflush(stdout);
    }
}
