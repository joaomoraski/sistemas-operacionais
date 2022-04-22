// Funcionalidade: Receber o dado do usuário e enviar para o p2-read
// Autores: João Vitor Moraski Lunkes, Flavio Bernaski, Caio Luiz dos Santos
// Data: 22/04/2022

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>

#define SERVER_FIFO "/tmp/addition_fifo_server"

char my_fifo_name [128];
char buf1 [512], buf2 [1024];

int main (int argc, char **argv)
{
    int fd_server;
    //    int fd, fd_server, bytes_read;

    // make client fifo
    sprintf (my_fifo_name, "/tmp/add_client_fifo%ld", (long) getpid ());

    if (mkfifo (my_fifo_name, 0664) == -1)
        perror ("mkfifo");

    while (1) {
        // get user input
        printf ("String: ");
        if (fgets(buf1, sizeof (buf1), stdin) == NULL)
            break;

        // send message to server

        if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) {
            perror ("open: server fifo");
            break;
        }

        if (write (fd_server, buf1, strlen (buf1)) != strlen (buf1)) {
            perror ("write");
            break;
        }

        if (close (fd_server) == -1) {
            perror ("close");
            break;
        }
    }

    return 0;
}
