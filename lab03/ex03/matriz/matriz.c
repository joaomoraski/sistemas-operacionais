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
#include "matriz.h"

/** cria uma matriz dinâmica row x col de inteiros */
int **create_matrix(int row, int col) {
    int **matrix = (int **) malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++)
        matrix[i] = (int *) malloc(col * sizeof(int));
    return matrix;
}

/** exibe a matriz de inteiros na saída padrão **/
void print_matrix(int **matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/** gera elementos inteiros aleatorios entre [1,b] para uma matriz row x col*/
void generate_elements(int **matrix, int row, int col, int limit) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            matrix[i][j] = rand() % limit + 1;
}

/** lê matriz de inteiros no formato MxN de um arquivo */
int **read_matrix_from_file(char *filename, int *row, int *col) {
    FILE *fd;
    int r, c, value;

    fd = fopen(filename, "r");
    fscanf(fd, "%dx%d\n", &r, &c);

    int **m = create_matrix(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            fscanf(fd, "%d", &m[i][j]);

    fclose(fd);

    printf("\nrow dos cara: %d\n", r);
    printf("\ncol dos cara: %d\n", c);
    *row = r;
    *col = c;
    return m;
}

void writeMatrizToFile(int row, int col) {
    FILE *file;
    char output[] = "entry_data.in"; // Nome de saida do arquivo
    int value;
    file = fopen(output, "w"); // Abrindo o arquivo em modo de escrita
    if (file) { // verifica se o arquivo foi aberto
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                value = rand() % 1000; // Valor aleatorio com limite de 1000
                fprintf(file, "%d ", value); // escrever no arquivo
            }
            fputs("\n", file); // finaliza uma linha
        }

    }
    fclose(file);
}
