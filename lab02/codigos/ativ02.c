/*
* O arquivo de código a seguir recebe um comando na execução do programa e o executa em um filho, aguardando sua finalização para terminar o processo principal
* Autor: João Moraski | RA: 1904000
* Data de criação: 12/04/2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
      // Verifica se o codigo esta sendo executado corretamente
      if (argc < 2) return printf("Informe o comando a ser executado\n");

      // Cria um vetor para armazenar os argumentos do comando
      char **cmd = malloc(sizeof(char *) * argc);

      // Preenche cmd com todos os parametros passado pela execução do programa
      // começa em 1 pq o primeiro argumento (0) de argv é o nome do arquivo executado
      for (int i = 1; i < argc; i++) {
        cmd[i-1] = argv[i];
      }
      
      // Passa null no final para que o execvp saiba que esta no final da lista.
      cmd[argc - 1] = NULL;

      pid_t pid = fork();
      if (pid == 0) {
          // Se for filho executa o comando
          // Execvp busca o executavel no caminho para vocë, facilitando a passagem do argumento, podendo ser apenas o nome do comando
          execvp(cmd[0], cmd);
      }

      // Espera a finalização
      wait(NULL);
      // Printa o comando que foi executado e finaliza o processo
      printf("O comando %s foi executado\n", cmd[0]);
}
