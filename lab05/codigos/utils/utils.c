#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void write_vectors_to_file(int* v1, int* v2, int* v3, int num_elements) {
    FILE* result_file = fopen("result.txt", "w");

    if (result_file == NULL) {
        printf("Error: Could not open file for writing\n");
        return;
    }

    fprintf(result_file, "Número de elementos: %d\n\n", num_elements);

    fprintf(result_file, "Resultados da soma dos vetores:\n");
    fprintf(result_file, "%-15s %-15s %-15s %-15s\n", "Índice", "Vetor 1", "Vetor 2", "Soma");
    fprintf(result_file, "-------------------------------------------------------\n");

    for (int i = 0; i < num_elements; i++) {
        fprintf(result_file, "%-15d %-15d %-15d %-15d\n", i, v1[i], v2[i], v3[i]);
    }

    fclose(result_file);
}
