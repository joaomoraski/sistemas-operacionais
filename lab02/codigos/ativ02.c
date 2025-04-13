#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
      if (argc < 2) return printf("Informe o comando a ser executado\n");

      char **cmd = malloc(sizeof(char *) * argc);

      for (int i = 1; i < argc; i++) {
        cmd[i-1] = argv[i];
      }
      cmd[argc - 1] = NULL;

      pid_t pid = fork();
      if (pid == 0) {
          execvp(cmd[0], cmd);
      }

      wait(NULL);
      printf("O comando %s foi executado\n", cmd[0]);
}
