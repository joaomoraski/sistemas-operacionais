/*
* O arquivo de código a seguir cria uma hierarquia de processos de tamanho N em função de 2. No caso, tendo 2 filhos em casa nível
* Autor: João Moraski | RA: 1904000
* Data de criação: 12/04/2025
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Função utilizada para criação da arvore de hierarquia de processos
// Utiliza do "actualLv" para guardar o nivel atual da arvore e o "maxLv" para acompanhar o limite
// actualLv começa em 1 porque o processo inicial(P) já é a atividade executada.
void createTree(int actualLv, int maxLv)
{
    if (actualLv == maxLv) return;


    // Aqui temos a criação dos 2 subniveis, como a arvore pede para que seja e função de 2 fazemos isso.
    // Só pede para criar mais caso seja o filho executando
    // Recursividade para fazer para cada nivel especificado.
    pid_t child = fork();
    if (child == 0)
    {
        createTree(actualLv + 1, maxLv);
        // Sleep para dar tempo de executar o pstree -p pid
        sleep(10);
        exit(0);
    }

    pid_t child2 = fork();
    if (child2 == 0)
    {
        createTree(actualLv + 1, maxLv);
        sleep(10);
        exit(0);
    }

    // Espera a finalização dos dois criados.
    wait(NULL);
    wait(NULL);
}

int main(int argc, char* argv[])
{
    // Verifica se o comando de execução foi passado corretamente
    if (argc < 2) return printf("Usage: %s [command]\n", argv[0]);

    // Printa o pid raiz para facilitar o pstree -p pid em outra tela
    printf("Pid raiz = %d\n", getpid());
    
    // Converte a string/char em número
    int number = atoi(argv[1]);
    // Passa para a função de criar a arvore
    createTree(1, number);
}
