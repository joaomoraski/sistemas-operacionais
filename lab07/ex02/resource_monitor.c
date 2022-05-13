#include "resource_monitor.h"
#include <unistd.h>
#include <stdio.h>

/* --- concurrency controls --- */
pthread_mutex_t the_mutex;
pthread_cond_t condBarbeiro, clienteWait;

/* --- resource --- */
int bufferCadeiraBarbeiro;
int bufferCliente[3];
int clienteCount;

// Initialize the mutex and condition variables
void initMonitor() {
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condBarbeiro, NULL);
    pthread_cond_init(&clienteWait, NULL);

    clienteCount = 0;
    for (int i = 0; i < 3; i++) bufferCliente[i] = -1;
    bufferCadeiraBarbeiro = -1;
}


//adiciona cliente se ainda tiver espaco
int put(int thread_id) {

    //evitar duplicacao, nao executar a funcao se a thread que a chamou ja tiver no buffer
    int flag = 0;
    for (int i = 0; i < clienteCount; i++)
        if (thread_id == bufferCliente[i])
            flag = 1;
    if (thread_id == bufferCadeiraBarbeiro) {
        flag = 1;
    }
    if (flag == 1) return 1;

    //programa a partir dessa linha
    pthread_mutex_lock(&the_mutex);

    // Se não houver clientes, chama o barbeiro e ocupa sua cadeira
    if (bufferCadeiraBarbeiro == -1) {
        bufferCadeiraBarbeiro = thread_id;
        clienteCount += 1;
        pthread_cond_signal(&condBarbeiro);
        // Se houver clientes, menos de 4, então o cliente fica em uma das cadeiras de espera
    } else if (clienteCount < 4) {
        bufferCliente[clienteCount] = thread_id;
        clienteCount += 1;
        //se não houver cadeira de espera, vai embora e não entra no buffer
    } else {
        printf("Cliente %d não achou lugar e foi embora.", thread_id);
    }

    pthread_mutex_unlock(&the_mutex); /* release the buffer */
    return 0;
}


//funcao exclusiva do barbeiro
int take() {
    pthread_mutex_lock(&the_mutex); /* protect buffer */

    //barbeiro esperando por qualquer chamado
    while (clienteCount < 1)             /* If there is something  in the buffer then wait */
        pthread_cond_wait(&condBarbeiro, &the_mutex);

    // desloca para esquerda os clientes que estão na cadeira de espera
    if (clienteCount >= 1) {
        // desloca para esquerda os clientes que estão na cadeira de espera
        for(int i=0; i<clienteCount; i++){
            bufferCliente[i] = bufferCliente[i+1];
        }
    }

    //reseta a ultima posição do bufferCliente, pois esta saindo da espera para a cadeira
    bufferCadeiraBarbeiro = bufferCliente[clienteCount];
    bufferCliente[clienteCount] = -1;
    clienteCount = clienteCount - 1;

    //Simula o tempo de corte
    sleep(3);
    //manda sinal para que possam se juntar no buffer de novo
    pthread_cond_signal(&clienteWait);
    pthread_mutex_unlock(&the_mutex); /* release the buffer */

    //retorna 1 se deu certo o corte
    return 1;
}


// Cleanup -- would happen automatically at end of program
void destroyMonitor() {
    pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
    pthread_cond_destroy(&condBarbeiro);      /* Free up consumer condition variable */
    pthread_cond_destroy(&clienteWait);      /* Free up consumer condition variable */
}