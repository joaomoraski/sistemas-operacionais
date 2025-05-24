/*
* Faça um programa que manipule arquivos (ler e escrever) e que, ao receber o signal 2 (SIGINT)
ou signal 15 (SIGTERM), faça uma finalização limpa (graceful stop) – armazenar as informações
pendentes e fechar o arquivo.
* Autor: João Moraski | RA: 1904000
* Data de criação: 23/05/2025
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Nome do arquivo
#define FILENAME "log.txt"
// Maximo de buffer pendente
#define MAX_PENDING_DATA 256

FILE* data_file = NULL;
volatile sig_atomic_t program_active = 1;
char pending_data[MAX_PENDING_DATA] = "";
#define MAX_INPUT_SIZE 100 // tamanho máximo para a entrada do usuario

// Tentei adicionar um sinal de alarm para interromper se eu nao forneço entrada, mas infelizmente ele simplesmente fica
// travado pq é uma chamada bloqueante e so termina realmente se eu dou enter

// Handler dos sinais
void graceful_shutdown(int sig_num) {
    printf("\nSignal %d received. Initiating graceful shutdown...\n", sig_num);

    if (data_file != NULL) {
        if (strlen(pending_data) > 0) {
            fprintf(data_file, "SAVED_ON_SHUTDOWN: %s\n", pending_data);
            fflush(data_file); // Garante que o dado esta salvo
        }
        fclose(data_file);
        data_file = NULL;
    }
    program_active = 0; // Muda para indicar ao loop que deve parar
}

int main() {
    char input_usuario[MAX_INPUT_SIZE];

    // configurar signal handler para o SIGINT e SIGTERM
    signal(SIGINT, graceful_shutdown);
    signal(SIGTERM, graceful_shutdown);

    printf("Program running. Press Ctrl+C or send SIGTERM to terminate.\n");

    // abrir o arquivo para leituar e escrita, caso nao exista ele cria
    data_file = fopen(FILENAME, "a+");
    if (data_file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (program_active) {
        // entrada do usuario
        printf("\nDigite uma mensagem (max %d caracteres): ", MAX_INPUT_SIZE - 1);
        if (fgets(input_usuario, MAX_INPUT_SIZE, stdin) == NULL) {
            perror("Erro ao ler entrada");
            break; // interrompe o loop para caso de erro
        }
        // removendo quebra de linha do fgets
        input_usuario[strcspn(input_usuario, "\n")] = '\0';
        strcpy(pending_data, input_usuario);

        // le o arquivo
        printf("\n--- Reading file ---\n");
        fseek(data_file, 0, SEEK_SET); // garante estar no inicio para ler
        char line_read[MAX_PENDING_DATA];
        while (fgets(line_read, sizeof(line_read), data_file) != NULL) {
            printf("%s", line_read);
        }
        printf("--- End of file read ---\n");
        printf("Pending data: '%s'\n", pending_data);

        // simulaçao de espera
        sleep(2);

        // Se o programa não foi parado, escreve o que esta pendente no arquivo
        if (program_active && strlen(pending_data) > 0) {
            fprintf(data_file, "RECORD: %s\n", pending_data);
            fflush(data_file); // farante que foi escrito
            pending_data[0] = '\0'; // limpa o conteudo pendente
        }
    }

    printf("Program terminated. Check '%s'.\n", FILENAME);
    return EXIT_SUCCESS;
}
