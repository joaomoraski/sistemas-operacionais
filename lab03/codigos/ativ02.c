/*
* Implemente um programa multithread com pthreads que calcule:
    a) a média aritmética de cada linha de uma matriz MxN e devolva o resultado em um vetor de tamanho M.
    b) a média geométrica de cada coluna de uma matriz MxN e devolva o resultado em um vetor de tamanho N.
    O programa deve gerar matrizes MxN com elementos aleatórios para arquivos; usar técnicas de
    paralelização de funções e de dados; ler matrizes MxN de arquivos no formato em anexo; gravar os
    resultados em um arquivo texto.
* Autor: João Moraski | RA: 1904000
* Data de criação: 22/04/2025
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "matriz/matriz.h"
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>

#define vMax 100

typedef struct {
    int start;
    int end;
    int number;
    int is_row;
    int row;
    int col;
    double* result;
    int** matrix;
} ThreadValues;

// matriz[linha][coluna]

// Função para calcular a média aritmética de cada linha
void* calc_arithmetic_mean(void* arg) {
    ThreadValues* data = (ThreadValues*)arg;

    if (data->is_row) {
        // Calcular a média aritmética das linhas
        for (int i = data->start; i < data->end; i++) {
            double sum = 0;
            for (int j = 0; j < data->col; j++) {
                sum += data->matrix[i][j];
            }
            data->result[i] = sum / data->col;
        }
    }
    else {
        // Calcular a média geométrica das colunas
        for (int i = data->start; i < data->end; i++) {
            double prod = 1.0;
            for (int j = 0; j < data->col; j++) {
                prod *= data->matrix[j][i];
            }
            data->result[i] = pow(prod, 1.0 / data->row);
        }
    }

    return NULL;
}

ThreadValues* mount_thread_values(int start, int end, int number, int is_row, int row, int col, int** matrix) {
    ThreadValues* data = malloc(sizeof(ThreadValues));
    double* result = malloc(sizeof(double) * col);
    data->start = start;
    data->end = end;
    data->number = number;
    data->is_row = is_row;
    data->row = row;
    data->col = col;
    data->result = result;
    data->matrix = matrix;
    return data;
}

void separate_and_calc(int num_threads, int row, int col, int** matrix, pthread_t threads[],
                       ThreadValues* threads_data_row[], ThreadValues* threads_data_col[]) {
    int range_per_thread_row = row / num_threads; // Define as variáveis de range
    int range_per_thread_col = col / num_threads; // Define as variáveis de range

    // Faz o cálculo e divisão de threads e ranges
    // Separei para ser N threads para cada problema, ou seja, se você passar 2, ele usará 2 Threads para cada.
    for (int i = 0; i < num_threads; i++) {
        // Start seria de onde vai o i * o range para cada thread, no caso 0 começa em zero, em 1, por exemplo, para 4 threads, iria para 25.
        int start = i * range_per_thread_row;
        // If ternário para definir se o final poderia estourar o limite ou não
        // Calculo do end é similar ao do start, porem com i + 1
        int end = (i + 1) * range_per_thread_row > row ? row : (i + 1) * range_per_thread_row;

        // Passa os valores para a struct | Media aritmetica das linhas
        ThreadValues* thread_values_row = mount_thread_values(start, end, i, 1, row, col, matrix);

        threads_data_row[i] = thread_values_row;

        start = i * range_per_thread_col;
        end = (i + 1) * range_per_thread_col > row ? row : (i + 1) * range_per_thread_col;
        // // Passa os valores para a struct | Media geometrica das colunas
        ThreadValues* thread_values_col = mount_thread_values(start, end, i, 0, row, col, matrix);
        threads_data_col[i] = thread_values_col;

        // Começa a criar as threads para fazer a divisão
        pthread_create(&threads[i], NULL, calc_arithmetic_mean, (void*)thread_values_row);
        pthread_create(&threads[i + num_threads], NULL, calc_arithmetic_mean, (void*)thread_values_col);
    }

    // Espera pela resposta dos filhos
    for (int i = 0; i < num_threads * 2; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) return printf("Informe o número de threads e o tamanho da matriz LxC \n");
    // Pega os valores da execução do arquivo
    int num_threads = atoi(argv[1]);
    printf("Numero de threads: %d\n", num_threads);
    int row = atoi(argv[2]);
    int col = atoi(argv[3]);
    int** matrix = create_matrix(row, col);
    generate_elements(matrix, row, col, vMax);

    clock_t time;
    time = clock();
    pthread_t threads[num_threads * 2];
    ThreadValues* threads_data_row[num_threads];
    ThreadValues* threads_data_col[num_threads];

    separate_and_calc(num_threads, row, col, matrix, threads, threads_data_row, threads_data_col);

    double* col_result = malloc(sizeof(double) * col);
    double* row_result = malloc(sizeof(double) * col);
    // Pega o resultado.
    for (int i = 0; i < num_threads; ++i) {
        col_result = threads_data_col[i]->result;
        row_result = threads_data_row[i]->result;
    }
    write_to_file(matrix, row_result, col_result, row, col);

    time = clock() - time;
    printf("Tempo percorrido nas operações de calculo com %d threads: %lds\n", num_threads, ((time) / CLOCKS_PER_SEC));
    // Limpa a memoria para evitar vazamento(nao iria ocorrer aq, mas boas praticas ne)
    for (int i = 0; i < num_threads; i++) {
        free(threads_data_col[i]);
        free(threads_data_row[i]);
    }
    free(col_result); // Incrível o que o linter do Clion fez, so tirou o warning quando coloquei esse free
    free(row_result);
}
