// Programa multiprocesso que gera 2 vetores aleatorios de N posicoes, soma os dois e gera um terceiro vetor com as respostas.
// Autores: João Vitor Moraski Lunkes, Caio Luiz dos Santos, Flavio Augusto Bernaski da Silva
// Data: 23/04
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>


#define PIPEREAD 0
#define PIPEWRITE 1


int main(int argc, char **argv) {
    int numberElements, numberProcess, indexDiv;
    if (argc > 3)
        return printf(
                "Número de parametros errado, informe apenas o numero de elementos do vetor e o numero de processos filhos.\n");
    if (argc == 2)
        return printf(
                "Número de parametros errado, informe apenas o numero de elementos do vetor e o numero de processos filhos.\n");
    if (argc == 1) {
        printf("Informe o numero de elementos no vetor: ");
        scanf("%d", &numberElements);
        printf("Informe o numero de processos filhos: ");
        scanf("%d", &numberProcess);
    } else {
        numberElements = atoi(argv[1]);
        numberProcess = atoi(argv[2]);
    }
    indexDiv = numberElements / numberProcess;
    printf("O algoritmo esta executando com um vetor de tamanho %d e usando %d processos\n", numberElements,
           numberProcess);

    // configuracao da memoria compartilhada
    const char *memoryName = "Lab05";
    const int moduloCharQtde = 3;
    const int memorySize = numberElements * 3 * (moduloCharQtde + 1) * sizeof(char) * 10;
    //tamanho do vetor * qtde de vetores (A, B e Resul) * caracteres que um numero possui;

    // criacao dos pipes
    int buffer;
    pid_t pid;
    int pipeToChild[numberProcess][2];
    int pipetoFather[numberProcess][2];
    for (int i = 0; i < numberProcess; ++i) {
        if (pipe(pipeToChild[i]) != 0) return printf("O programa falhou criando o Pipe to Child");
        if (pipe(pipetoFather[i]) != 0) return printf("O programa falhou criando o Pipe to Father");
    }

    for (int i = 0; i < numberProcess; ++i) {
        pid = fork();
        if (!pid) {
            pid = (pid_t) i;
            break;
        }
    }

    if (pid < numberProcess) {
        int startVetor, finishVetor, signal = 0;

        close(pipeToChild[pid][PIPEWRITE]);
        read(pipeToChild[pid][PIPEREAD], &startVetor, sizeof(startVetor));
        read(pipeToChild[pid][PIPEREAD], &finishVetor, sizeof(finishVetor));
        int doWhile = 10;
        do {
            // evitar concorrencia
            sleep(pid);

            doWhile--;

            int sharedMem = shm_open(memoryName, O_RDWR, 0666);

            // Se acontece erro na concorrencia ele tenta novamente
            if (sharedMem == -1) {
                if (doWhile - 1 == 0) return printf("Erro no shm_open: Processo %d falhou\n", pid);
                sleep(1);

                // Se for sucesso ele abre a memoria compartilhada
            } else {
                // Acessa memoria compartilhada
                void *ptr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0);

                // Criar os 3 vetores a partir do memoria compartilhada
                int vtr[3][numberElements];
                char tmpBuffer[10]; //buffer temporario
                int cont = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < numberElements; j++) {
                        sscanf(ptr + cont, "%s", tmpBuffer);
                        cont += strlen(tmpBuffer) + 1;
                        vtr[i][j] = atoi(tmpBuffer);
                    }
                }

                // Somar vetor A + B
                int res[numberElements];
                for (int i = startVetor; i <= finishVetor; i++)
                    res[i] = vtr[0][i] + vtr[1][i];

                // Inserir o resultado no shared memory
                cont = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < numberElements; j++) {
                        if (i == 2 && j >= startVetor && j <= finishVetor) {
                            cont += sprintf(ptr + cont, "%d ", res[j]);
                        } else {
                            cont += sprintf(ptr + cont, "%d ", vtr[i][j]);
                        }
                    }
                }

                //sinal de finalizacao
                signal = 1;
                close(pipetoFather[pid][PIPEREAD]);
                write(pipetoFather[pid][PIPEWRITE], &signal, sizeof(signal));
                return printf("Filho %d (pid: %d) terminou a execucao\n", (int) pid, getpid());
            }
        } while (doWhile > 0);
    }

    // Criar memoria compartilhada
    int sharedMem = shm_open(memoryName, O_CREAT | O_RDWR, 0666);
    ftruncate(sharedMem, memorySize);

    // Mapeia a memoria compartilhada para ptr
    void *ptr = mmap(0, memorySize, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0);
    if (ptr == MAP_FAILED) return printf("Map failed\n");

    // Controle de concorrencia

    // Insere os Vetores na shared memory
    int cont = 0;
    int modulo = (int) pow(10, moduloCharQtde);
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < numberElements; j++)
            cont += sprintf(ptr + cont, "%d ", rand() % modulo);

    for (int j = 0; j < numberElements; j++)
        cont += sprintf(ptr + cont, "0 ");

    // Controle de concorrencia
    //munlockall();

    //Distribuir os indices do vetor para cada filho
    for (int i = 0; i < numberProcess; i++) {
        close(pipeToChild[i][PIPEREAD]);

        //enviar indice inicio
        buffer = i * (numberElements / numberProcess);
        write(pipeToChild[i][PIPEWRITE], &buffer, sizeof(buffer));

        //enviar indice final
        buffer = ((i + 1) * (numberElements / numberProcess)) - 1;
        if (i + 1 == numberProcess) buffer = numberElements - 1;
        write(pipeToChild[i][PIPEWRITE], &buffer, sizeof(buffer));
    }

    //Receber o sinal de finalizacao de processo de cada filho
    for (int i = 0; i < numberProcess; i++) {
        close(pipetoFather[i][PIPEWRITE]);
        read(pipetoFather[i][PIPEREAD], &buffer, sizeof(buffer));
        if (buffer != 1) return printf("Pai: Filho %d falhou\n", i);
        printf("Pai: Filho %d retornou sinal de finalizacao\n", i);
    }

    // Criar os 3 vetores a partir do shared memory
    int vtr[3][numberElements];
    char temporaryBuffer[10]; //buffer temporario
    cont = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < numberElements; j++) {
            sscanf(ptr + cont, "%s", temporaryBuffer);
            cont += strlen(temporaryBuffer) + 1;
            vtr[i][j] = atoi(temporaryBuffer);
        }
    }

    // resultados
    printf("Vetor com os Resultados: [");
    for (int i = 0; i < numberElements - 1; i++) {
        printf("%d, ", vtr[2][i]);
    }
    printf("%d]\n", vtr[2][numberElements - 1]);

    // unlink shared memory
    if (shm_unlink(memoryName) == -1) {
        printf("Error removing %s\n", memoryName);
        exit(-1);
    }

    wait(NULL);
    printf("Pai terminou de executar\n");
    return 0;
}