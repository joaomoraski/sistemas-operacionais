#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define vMax 100

void generateRandomVector(int *vector, int target) {
    for (int i = 0; i < vMax; i++) {
        vector[i] = rand() % 100 + 1;
    }
}

int searchInVector(int *vector, int target, int start, int end) {
    int found = 0;
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

    int numProcesses = atoi(argv[1]);
    int target = atoi(argv[2]);
    int vector[vMax] = {};

    generateRandomVector(vector, target);

    int start, end, rangePerProcess;

    rangePerProcess = vMax/numProcesses;
    int foundByAny = 0;

    for (int i = 0; i < numProcesses; i++) {
        start = i * rangePerProcess;
        end = (i + 1) * rangePerProcess > vMax ? vMax : (i + 1) * rangePerProcess;
        pid_t pid = fork();
        if (pid == 0) { // fazer apenas o filho chamar
            int found = searchInVector(vector, target, start, end);
            exit(found ? 0 : 1);
        }
    }

    for (int i = 0; i < numProcesses; i++) {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            foundByAny = 1;
        }
    }

    if (!foundByAny) {
        printf("Pai (PID %d): Elemento %d não encontrado no vetor.\n", getpid(), target);
    }


}
