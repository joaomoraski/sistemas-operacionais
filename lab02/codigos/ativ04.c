/*
* Simula um breve terminal, aceitando a entrada com & para execução em background
* Autor: João Moraski | RA: 1904000
* Data de criação: 12/04/2025
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Define o maximo de argumentos e tamanho de input
#define MAX_INPUT 1024
#define MAX_ARGS 100

int main() {
	char input[MAX_INPUT];

	while (1) {
		printf("~ "); // Homenagem ao zsh que uso
		// Garante que vai estar limpo o buffer de saida
		fflush(stdout);

		// Le a entrada do usuário, caso de algum problema ou ele termine a entrada(Control + D) ele da break na entrada
		if (!fgets(input, sizeof(input), stdin)) {
			break;
		}

		input[strcspn(input, "\n")] = 0; // remover \n no final da linha

		if (strlen(input) == 0) continue;  // ignorar entradas vazias

		if (strcmp(input, "exit") == 0) { // verifica se é exit, ai sai
			break;
		}

		char *args[MAX_ARGS];
		int argc = 0;
		char *token = strtok(input, " ");  // split da entrada nos espaços
		int background = 0;

		while (token != NULL && argc < MAX_ARGS - 1) {
			if (strcmp(token, "&") == 0) { // se for backgroound sinaliza para o codigo
				background = 1;
			} else {
				args[argc++] = token; // se não for, adiciona o token para o vetor de args
			}
			// Split pelo espaço em branco
			token = strtok(NULL, " ");
		}
		// Passa null no final pela necessidade do execv
		args[argc] = NULL;

		pid_t pid = fork();

		if (pid == 0) {
			if (background) {
				// remover a saída padrão e erro para /dev/null se for para background
				// A ideia aq é impedir que apareça no terminal o que ta me background
				// Foi mal professor, mas essa eu pedi do chatgpt mesmo, nao consegui fazer sozinho
				int null_fd = open("/dev/null", O_WRONLY);
				dup2(null_fd, STDOUT_FILENO);
				dup2(null_fd, STDERR_FILENO);
				close(null_fd);
			}
			// Executa o comando
			execvp(args[0], args); // Caso nao de errado ele assume o papel do filho e executa o comando e finaliza
			// Como o execvp substitui o filho, caso de erro na execução ele retorna como 1 para indicar erro ao pai
			exit(1);
		} else if (pid > 0) { // Se for o pai
			if (!background) { // Se n for backgroound espera aexecução terminar
				waitpid(pid, NULL, 0);
			} else { // Se for background ele apenas print o que esta rodando e o pid
				printf("[Rodando em background - PID %d]\n", pid);
			}
		}
	}
	return 0;
}