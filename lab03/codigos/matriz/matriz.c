/**
 * Code: matriz.c (Header: matriz.h) 
 * Descrição: Implementação de funções básicas para manipular
 * matriz de inteiros dinâmica.
 * 
 * Autor: Rodrigo Campiolo
 * Data: 04/09/2019
 */


#include <stdio.h>
#include <stdlib.h>     //rand()
#include "../matriz/matriz.h"

/** cria uma matriz dinâmica row x col de inteiros */
int** create_matrix(int row, int col) {
    int** matrix = (int**)malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++)
        matrix[i] = (int*)malloc(col * sizeof(int));
    return matrix;
}

/** exibe a matriz de inteiros na saída padrão **/
void print_matrix(int** matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/** gera elementos inteiros aleatorios entre [1,b] para uma matriz row x col*/
void generate_elements(int** matrix, int row, int col, int limit) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix[i][j] = rand() % limit + 1;
        }
    }
}

/** lê matriz de inteiros no formato MxN de um arquivo */
int** read_matrix_from_file(char* filename, int* row, int* col) {
    FILE* fd;
    int r, c, value;

    fd = fopen(filename, "r");
    fscanf(fd, "%dx%d\n", &r, &c);

    int** m = create_matrix(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            fscanf(fd, "%d", &m[i][j]);

    fclose(fd);

    *row = r;
    *col = c;
    return m;
}

// Função para gravar a matriz e os resultados no arquivo
void write_to_file(int** matrix, double* row_result, double* col_result, int row, int col) {
    FILE* result_file = fopen("result.txt", "w");
    FILE* matrix_file = fopen("generated_matrix.txt", "w");

    if (result_file == NULL || matrix_file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escrever as dimensões
    fprintf(matrix_file, "%d %d\n", row, col);

    // Escrever a matriz
    fprintf(matrix_file, "Matriz:\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            fprintf(matrix_file, "%d ", matrix[i][j]);
        }
        fprintf(matrix_file, "\n");
    }

    // Escrever os resultados da média aritmética
    fprintf(result_file, "\nMédia aritmética das linhas:\n");
    for (int i = 0; i < row; i++) {
        fprintf(result_file, "%.2f ", row_result[i]);
    }
    fprintf(result_file, "\n");

    // Escrever os resultados da média geométrica
    fprintf(result_file, "\nMédia geométrica das colunas:\n");
    for (int i = 0; i < col; i++) {
        fprintf(result_file, "%.2f ", col_result[i]);
    }
    fprintf(result_file, "\n");

    fclose(matrix_file);
    fclose(result_file);
}
