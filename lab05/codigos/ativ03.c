/*
* 3. Implemente um programa que realize a soma de vetores utilizando processos para fazer o cálculo, mas
com os vetores sendo compartilhados pelos processos. Como os espaços de memória entre os processos
são isolados, um mecanismo fornecido pelo SO deve ser usado. No caso, use memória compartilhada
para que todos os filhos operem sobre os dados, e pipes para a realização do despacho de trabalho
(intervalo de índices no vetor). O número de elementos do vetor e o número de processos filhos deve ser
fornecido pelo usuário. Por exemplo, numElementos = 1000 e numProcessos = 5, cada filho processará
200 índices; para numElementos = 1000 e numProcessos = 4, cada filho processará 250 índices.
* Autor: João Moraski | RA: 1904000
* Data de criação: 24/05/2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#include "utils/utils.h"

void read_entry(int argc, char* argv[], int* elements_num, int* process_num) {
    if (argc < 3) {
        printf("Informe a quantidade de elementos no vetor: ");
        scanf("%d", elements_num);
        printf("Informe a quantidade de processos: ");
        scanf("%d", process_num);
    } else {
        *elements_num = atoi(argv[1]);
        *process_num = atoi(argv[2]);
    }
}

void initialize_vectors(int* v1, int* v2, int size) {
    for (int i = 0; i < size; i++) {
        v1[i] = rand() % 100;
        v2[i] = rand() % 100;
    }
}

int main(int argc, char* argv[]) {
    int num_elements = 0, num_processes = 0;

    read_entry(argc, argv, &num_elements, &num_processes);

    if (num_elements <= 0 || num_processes <= 0) {
        printf("ERRO: valores inválidos fornecidos.\n");
        return EXIT_FAILURE;
    }

    // calculo de trabalho por processo
    int work_per_process = num_elements / num_processes;

    // criaçao da memoria compartilhada
    int* shared_mem = mmap(NULL, sizeof(int) * num_elements * 3, PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }
    int* v1 = shared_mem; // inicio do bloco de memoria
    int* v2 = shared_mem + num_elements; // inicio + num_elementos
    int* v3 = shared_mem + num_elements * 2; // inicio + num_elements * 2

    // Inicializar vetores V1 e V2
    initialize_vectors(v1, v2, num_elements);

    // memoria sinalizadora
    // usada para permitir que os filhos informem ao pai que concluiu o trabalho
    // e sincronizar pai e filho para ele so prosseguir dps de ter certeza
    // to estudando pelos slides e fazendo o laboratorio, ai to anotando aqui, tamo junto campiolo
    int* signal_mem = mmap(NULL, sizeof(int) * num_processes, PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (signal_mem == MAP_FAILED) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }
    memset(signal_mem, 0, sizeof(int) * num_processes);

    // Criar pipes para comunicação
    int pipes[num_processes][2];
    for (int i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Failed to create pipe");
            return EXIT_FAILURE;
        }
    }

    // processos filhos
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) { // filho
            close(pipes[i][1]); // fechar a escrita

            // ler a pipe
            int start, end;
            read(pipes[i][0], &start, sizeof(int));
            read(pipes[i][0], &end, sizeof(int));
            close(pipes[i][0]); // fechar apos leitura

            // realizar as somas desse processo
            for (int j = start; j < end; j++) {
                v3[j] = v1[j] + v2[j];
            }

            // sinal que terminou
            signal_mem[i] = 1;

            // finaliza
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("Failed to fork process");
            return EXIT_FAILURE;
        }
        // pai
    }

    // envio dos dados pros filhos
    for (int i = 0; i < num_processes; i++) {
        close(pipes[i][0]); // fecha a leitura

        // define o intervalo
        int start = i * work_per_process;
        int end = (i == num_processes - 1) ? num_elements : start + work_per_process;

        // escreve na pipe
        write(pipes[i][1], &start, sizeof(int));
        write(pipes[i][1], &end, sizeof(int));
        close(pipes[i][1]); // fecha a escrita
    }

    // esperar os fio
    for (int i = 0; i < num_processes; i++) {
        while (signal_mem[i] == 0) {
            usleep(1000); // Espera ativa
        }
    }

    write_vectors_to_file(v1, v2, v3, num_elements);
    printf("Resultados salvos no arquivo 'result.txt'\n");


    // desalocaçao de memoria
    munmap(shared_mem, sizeof(int) * num_elements * 3);
    munmap(signal_mem, sizeof(int) * num_processes);

    return EXIT_SUCCESS;
}