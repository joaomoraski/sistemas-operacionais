/*
* Gera um vetor aleatorio e faz a busca de algum número escolhido na execução, a busca é feita de forma dividida entre N processos, também passados na execução
* Autor: João Moraski | RA: 1904000
* Data de criação: 12/04/2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define vMax 100

// Gera um vetor aleatório de tamanho vMax(100)
void generateRandomVector(int *vector, int target) {
    for (int i = 0; i < vMax; i++) {
        vector[i] = rand() % 100 + 1;
    }
}

int searchInVector(int *vector, int target, int start, int end) {
    // variavel de controle para informar ao pai se foi encontrado ou não
    int found = 0;
    // Faz a busca do valor em seu especifico range
    for (int i = start; i < end; i++) {
        if (vector[i] == target) {
            printf("O elemento %d foi encontrado pelo filho de PID: %d\n", vector[i], getpid());
            found = 1;
        }
    }
    return found;
}

int main(int argc, char* argv[]) {
    if (argc < 3) return printf("Informe o número de threads e o número a ser buscado\n");

    // Pega os valores da execução do arquivo
    int numProcesses = atoi(argv[1]);
    int target = atoi(argv[2]);
    int vector[vMax] = {}; // Inicializa o vetor

    generateRandomVector(vector, target);

    int start, end, rangePerProcess; // Define as variaveis de range

    rangePerProcess = vMax/numProcesses;
    int foundByAny = 0; // Variavel de controle que sera usada para validar se foi encontrado ou não

    // Faz o calculo e divisão de processos e ranges
    for (int i = 0; i < numProcesses; i++) {
        // Start seria de onde vai o i * o range para cada processo, no caso 0 começa em zero, em 1, por exemplo, para 4 processos, iria para 25.
        start = i * rangePerProcess;
        // If ternario para definir se o final poderia estourar o limite ou não
        // Calculo do end é similar ao do start, porem com i + 1
        end = (i + 1) * rangePerProcess > vMax ? vMax : (i + 1) * rangePerProcess;
        // Começa a criar os processos para fazer a divisao
        pid_t pid = fork();
        if (pid == 0) { // fazer apenas o filho chamar
            int found = searchInVector(vector, target, start, end);
            // Retorna com 1 para o pai caso não tenha encontrado o numero 
            exit(found ? 0 : 1);
        }
    }

    // Espera pela resposta dos filhos
    for (int i = 0; i < numProcesses; i++) {
        int status;
        wait(&status); // Espera o filho terminar e pega o status dele
        // Verifica primeiro se o filho foi terminado normalmente ou por algum sigkill
        // E depois realmente extrai o codigo de saida para verificar se teve 0 ou não
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // Se passou pelas validações, foi encontrado, entao seta o valor do pai
            foundByAny = 1;
        }
    }

    // Avisa se não foi encontrado
    if (!foundByAny) {
        printf("Pai (PID %d): Elemento %d não encontrado no vetor.\n", getpid(), target);
    }


}
