/*
* 4. a) Faça dois programas para atuarem respectivamente como cliente e servidor para um serviço de tradução
simples (só traduz palavras). O serviço de tradução deve receber um código especificando a língua de
origem e destino (use o padrão ISO 639-1) e a palavra para traduzir (ex: pt-en:cachorro). Se não
conseguir fazer a tradução devolve ERROR:UNKNOWN, caso contrário, a tradução. Por exemplo,
traduzir de pt-en (português para o inglês) a palavra cachorro, devolve dog.
Faça uma versão usando sockets UNIX e que delega o processamento para um processo filho, isto
é, o cliente pode continuar enviando mensagens para traduzir até que envie a mensagem NO-NO
como código.
* Autor: João Moraski | RA: 1904000
* Data de criação: 24/05/2025
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/translate_socket"
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    // criaçao do socket
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erro ao criar socket");
        exit(1);
    }

    // configuraçao do endereço de memroia
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // conecta os sockets
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao conectar");
        exit(1);
    }

    printf("Conectado ao servidor de tradução!\n");
    printf("Digite o código da língua e a palavra (ex: pt-en:cachorro)\n");
    printf("Digite 'NO-NO' para sair\n\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remoção do \n

        // envia a mensagem
        send(client_socket, buffer, strlen(buffer), 0);

        // para se for para sair
        if (strcmp(buffer, "NO-NO") == 0) break;

        // recebe resposta
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Conexão encerrada pelo servidor\n");
            break;
        }

        buffer[bytes_received] = '\0';
        // printa tradução
        printf("Tradução: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}