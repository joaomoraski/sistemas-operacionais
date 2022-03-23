//
// Created by moraski on 20/03/2022.
//
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "sys/wait.h"
#include "math.h"

int *random_vector(int n, int max, int seed) {
    int *vector = malloc(sizeof(int) * n);
    srand(seed);
    for (int i = 0; i < n; ++i) {
        vector[i] = rand() % max;
    }
    return vector;
}

int main(int argc, char **argv) {
    if (argc > 3) return printf("Quantidade de parametros errada");
    if (argc < 2) printf("Informe o numero de filhos e o valor a procurar na execucao do programa");
    if (atoi(argv[1]) > 15) return 0;

    int tamVetor = pow(10, 4), status, inicio, fim;
    int *vector = random_vector(tamVetor, 100, 42);
    int numeroProcurado = atoi(argv[2]), numeroFilhosTotal = atoi(argv[1]), pid, filhosCriados = 0;


    for (int i = 0; i < numeroFilhosTotal; ++i) {
        pid = fork(); // cria o 1 filho
        if (!pid) { // se for filho ele continua para criacao do resto, se nao ele quebra
            filhosCriados++;
            if (filhosCriados > numeroFilhosTotal) break; // caso o numero de filhos venha a ultrapassar o total
        } else {
            break;
        }
    }

    if (filhosCriados == 1) { // Se for o 1 filho a ser criado ele comeca do 0
        inicio = 0;
    } else {
        inicio = (tamVetor / numeroFilhosTotal) * filhosCriados; // se nao calcula o inicio com base de quantos filhos criado
        // Vetor de 100 numeros, 2 filho criado comecaria pelo 25 ate o 50
    }
    fim = (tamVetor / numeroFilhosTotal) * (filhosCriados + 1);  // calcula o fim com base na mesma logica de cima porem com +1 no filhos criados para achar o fim


    if (fim > tamVetor) fim = tamVetor; // para evitar um null pointer, se o fim for maior que o vetor ele automaticamente recebe o tamanho do vetor

    for (int i = inicio; i < fim; ++i) { // Percorre o vetor do inicio ao fim ja pre-definido para aquele processo
        if (vector[i] == numeroProcurado) {
            printf("O %d° filho com o PID: %d conseguiu achar o valor no indice %d do vetor.\n", filhosCriados + 1,
                   getpid(), i);
            // + 1 para que possa aparecer o numero do filho certo na log
        }
    }

    free(vector);
    wait(&status);

    return 0;
}