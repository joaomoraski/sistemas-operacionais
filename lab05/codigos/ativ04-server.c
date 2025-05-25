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
#include <sys/wait.h>

#define SOCKET_PATH "/tmp/translate_socket"
#define BUFFER_SIZE 1024

struct Translation {
    char lang_pair[6];  // pt-en
    char word[50];
    char translation[50];
};

struct Translation dictionary[] = {
    {"pt-en", "cachorro", "dog"},
    {"pt-en", "gato", "cat"},
    {"pt-en", "casa", "house"},
    {"en-pt", "dog", "cachorro"},
    {"en-pt", "cat", "gato"},
    {"en-pt", "house", "casa"},
};

void translate(char* request, char* response) {
    char lang_pair[6];
    char word[50];
    char* token;

    // usa strtok para separar a string no ':'
    token = strtok(request, ":");
    if (token != NULL) {
        // lê o código do idioma (limitado a 5 caracteres)
        strncpy(lang_pair, token, 5);

        // lê a palavra
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strncpy(word, token, 49);
        }
    }

    // verifica se tem as duas partes
    if (lang_pair[0] == '\0' || word[0] == '\0') {
        strcpy(response, "ERROR:UNKNOWN");
        return;
    }

    // pega a tradução do dicionario
    int dict_size = sizeof(dictionary) / sizeof(struct Translation);
    for (int i = 0; i < dict_size; i++) {
        if (strcmp(dictionary[i].lang_pair, lang_pair) == 0 &&
            strcmp(dictionary[i].word, word) == 0) {
            strcpy(response, dictionary[i].translation);
            return;
        }
    }

    strcpy(response, "ERROR:UNKNOWN");
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        // pega a requisição
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';

        // verifica se é para parar e interrompe
        if (strcmp(buffer, "NO-NO") == 0) {
            close(client_socket);
            // mata o processo pai
            kill(getppid(), SIGTERM);
            exit(0);
        }


        // faz a tradução
        translate(buffer, response);

        // envia resposta
        send(client_socket, response, strlen(response), 0);
    }

    close(client_socket);
    exit(0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_un server_addr;

    // remove o socket caso ja exista
    unlink(SOCKET_PATH);

    // criaçao do socker
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erro ao criar socket");
        exit(1);
    }

    // configuração do endereço
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // da bind no endereço
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro no bind");
        exit(1);
    }

    // fica escutando as conexões
    if (listen(server_socket, 5) == -1) {
        perror("Erro no listen");
        exit(1);
    }

    printf("Servidor de tradução iniciado...\n");

    while (1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Erro no accept");
            continue;
        }

        // processo filho para lidar com client
        pid_t pid = fork();
        if (pid == 0) {
            // filho
            close(server_socket);
            handle_client(client_socket);
        } else {
            // pai
            close(client_socket);
        }
    }
    return EXIT_SUCCESS;
}
