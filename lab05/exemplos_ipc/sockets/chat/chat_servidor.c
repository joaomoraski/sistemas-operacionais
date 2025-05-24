/*------------------------------------------------------------------------
 * Programa:
 *    chat_servidor - Servidor Chat
 *
 * Funcionalidades:
 *    - alocar um socket para receber conex�es
 *    - limitar o numero de conex�es
 *    - mapear o processo a uma porta
 *    - aguardar as conex�es
 *    - iniciar um socket para a comunica��o com o cliente
 *    - enviar/receber as msg
 *
 *
 * Sintaxe:    chat_servidor [porta]
 *
 *    - porta - n�mero da porta que o servidor est� usando
 *
 * Notas:
 *    - se porta n�o for fornecida � usado um valor de porta padr�o
 *------------------------------------------------------------------------
 */

#include <sys/types.h>              /* tipos b�sicos do GNU C */
#include <sys/socket.h>             /* fun��es, tipos e constantes para sockets (UNIX) */
#include <netinet/in.h>             /* fun��es, tipos e constantes para sockets (Internet) */
#include <netdb.h>                  /* dados sobre a rede (ex.: hosts, protocolos, ...) */

#include <stdio.h>                  /* entrada/sa�da (I/O) */
#include <string.h>                 /* manipula��o de strings */
#include <stdlib.h>                 /* exit() */
#include <unistd.h>                 /* close() */

#define IP_SERVER     "127.0.0.1"   /* especifica o IP do servidor */
#define PORTA_PADRAO  5000          /* especifica a porta padrao de conexao */
#define MAX_CLIENT    10            /* maximo de conexoes */
#define MAX_BUFFER    100           /* tamanho m�ximo do buffer */

int main(int argc, char *argv[]) {

	struct hostent *ptrh; /* ponteiro para a tabela de hosts */
	struct sockaddr_in addr_server, /* estrutura para armazenar o IP e a porta do servidor */
	addr_client; /* estrutura para armazenar o endereco do cliente */
	int serverSocket, /* socket para ouvir conex�es */
	clientSocket; /* socket para comunica��o com o jogador conectado */
	int port; /* porta utilizada na comunica��o */
	char buffer[MAX_BUFFER]; /* armazena os dados recebidos */

	/* verifica os par�metros passados por linha de comando */
	if (argc > 1) port = atoi(argv[1]); /* converte para inteiro */
	else port = PORTA_PADRAO; /* usa a porta padr�o */

	/* testa a validade da porta */
	if ((port <= 1024) || (port >= 65536)) {
		fprintf(stderr,"n�mero de porta inv�lido %s\n",argv[2]);
		exit(1);
	} //if

	/* obtem o endereco do servidor (isto �, o endere�o local) */
	ptrh = gethostbyname(IP_SERVER);

	/* preenche com zeros a estrutura sockaddr_in */
	memset((char *)&addr_server,0,sizeof(addr_server));

	/* preenche a estrutura endereco */
	addr_server.sin_family = AF_INET; /* define familia como Internet  */
	addr_server.sin_port = htons(port); /* define a porta */
	addr_server.sin_addr.s_addr = INADDR_ANY; /* seta o endereco IP local */
	//memcpy(&addr_server.sin_addr, ptrh->h_addr, ptrh->h_length);   /* especifica o IP */

	/* cria um socket servidor TCP */
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	/* testa para verificar sucesso */
	if (serverSocket < 0) {
		fprintf(stderr, "falha na cria��o do socket\n");
		exit(1);
	} //if

	/* mapeia um endereco local para o socket */
	if (bind(serverSocket, (struct sockaddr *)&addr_server, sizeof(addr_server)) < 0) {
		fprintf(stderr,"erro no mapeamento da porta (bind)\n");
		exit(1);
	} //if

	/* Especifica o numero de conexoes suportadas - backlog */
	if (listen(serverSocket, MAX_CLIENT) < 0) {
		fprintf(stderr,"erro em limitar o n�mero de conex�es (listen)\n");
		exit(1);
	} //if

	while (1) {
		/* Aguarda por conexoes */
		int szAddressClient = sizeof(addr_client);
		printf ("Servidor iniciado e aguardando conex�es ...\n");
		if ( (clientSocket=accept(serverSocket, (struct sockaddr *)&addr_client,(socklen_t *) &szAddressClient)) < 0) {
			fprintf(stderr, "erro ao aceitar conex�o (accept)\n");
			exit(1);
		} // if

		/* protocolo de comunica��o (enviar e receber dados) */
		recv (clientSocket, buffer, sizeof (buffer), 0); /* recebe dados */
		strcpy (buffer,"Conexao aceita.\n");
		send (clientSocket, buffer, sizeof (buffer), 0); /* envia dados */

		buffer[MAX_BUFFER-1] = 0x0;
		printf ("%s\n", buffer);
		printf ("Cliente conectado \n\n");

		/* protocolo para o chat */
		printf ("------------------------------------------------\n");
		printf ("--               Chat Servidor                --\n");
		printf ("------------------------------------------------\n\n");

		while (1) {
			recv (clientSocket, buffer, sizeof (buffer), 0); /* recebe dados */
			printf ("Mensagem (Cliente): %s\n", buffer);

			if (strcmp(buffer, "sair") == 0) break;

			printf("Mensagem (Servidor): ");
			gets(buffer);
			send (clientSocket, buffer, sizeof (buffer), 0); /* envia dados */
		} //while

		/* fecha a conex�o */
		close(clientSocket);

	} //while

	close(serverSocket);

	/* finaliza o programa */
	exit(0);
} // main
