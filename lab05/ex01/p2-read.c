// Funcionalidade: Le o que é enviado pelo write e fazer o tratamento para que retorne
// O tamanho, numero de vogais, consoantes e o número de espaços
// Autores: João Vitor Moraski Lunkes, Flavio Bernaski, Caio Luiz dos Santos
// Data: 22/04/2022

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/addition_fifo_server"
#define MAX_NUMBERS 500

int getNumeroEspaco(char palavra[40]) {
    int contEspaco = 0;
    for (int i = 0; i < strlen(palavra); ++i) {
        if (palavra[i] == ' ') {
            contEspaco++;
        }
    }
    return contEspaco;
}

int getVogais(char palavra[40]) {
    char vogais[10] = {'A', 'E', 'I', 'O', 'U', 'a', 'e', 'i', 'o', 'u'};
    int contVogais = 0;
    for (int i = 0; i < strlen(palavra); ++i) {
        for (int j = 0; j < 10; ++j) {
            if (palavra[i] == vogais[j]) {
                contVogais++;
            }
        }
    }
    return contVogais;
}

int getConsoantes(char palavra[40]) {
    char * consoantes[42] = {"B","C","D","F","G","H","J","K","L","M","N","P","Q","R","S","T","V","W","X","Y","Z",
                       "b","c","d","f","g","h","j","k","l","m","n","p","q","r","s","t","v","w","x","y","z"};
    int contConsoantes = 0;
    for (int i = 0; i < strlen(palavra); ++i) {
        for (int j = 0; j < 42; ++j) {
            if (palavra[i] == *consoantes[j]) {
                contConsoantes++;
            }
        }
    }
    return contConsoantes;
}


int main (int argc, char **argv)
{
    int fd, bytes_read;
    char buf [4096];


    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) {
        perror ("mkfifo");
        exit (1);
    }
    if ((fd = open (SERVER_FIFO, O_RDONLY)) == -1)
        perror ("open");


    while (1) {
        // get a message
        memset (buf, '\0', sizeof (buf));

        if ((bytes_read = read (fd, buf, sizeof (buf))) == -1)
            perror ("read");
        if (bytes_read == 0)
            continue;

        if (bytes_read > 0) {
            printf("____________________________________________________\n");
            printf("String: %s", buf);
            printf("Tamanho da string: %lu\n", strlen(buf)-1);
            printf("Número de consoantes: %d\n", getConsoantes(buf));
            printf("Número de vogais: %d\n", getVogais(buf));
            printf("Número de espaços: %d\n", getNumeroEspaco(buf));
            printf("____________________________________________________\n\n");
        }
    }
}
