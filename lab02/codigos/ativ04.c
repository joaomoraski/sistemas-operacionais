#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

int main() {
	char input[MAX_INPUT];

	while (1) {
		printf("~ "); // Homenagem ao zsh que uso
		fflush(stdout);

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
				args[argc++] = token; // se não for, adiciona o token
			}
			token = strtok(NULL, " ");
		}
		args[argc] = NULL;

		pid_t pid = fork();

		if (pid == 0) {
			if (background) {
				// remover a saída padrão e erro para /dev/null
				// A ideia aq é impedir que apareça no terminal o que ta me background
				int null_fd = open("/dev/null", O_WRONLY);
				dup2(null_fd, STDOUT_FILENO);
				dup2(null_fd, STDERR_FILENO);
				close(null_fd);
			}
			execvp(args[0], args);
			exit(1);
		} else if (pid > 0) {
			if (!background) {
				waitpid(pid, NULL, 0);
			} else {
				printf("[Rodando em background - PID %d]\n", pid);
			}
		}
	}
	return 0;
}