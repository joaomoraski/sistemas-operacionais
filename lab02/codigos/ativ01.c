#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void createTree(int actualLv, int maxLv)
{
    if (actualLv == maxLv) return;

    pid_t child = fork();
    if (child == 0)
    {
        createTree(actualLv + 1, maxLv);
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

    wait(NULL);
    wait(NULL);
}

int main(int argc, char* argv[])
{
    if (argc < 2) return printf("Usage: %s [command]\n", argv[0]);

    printf("Pid raiz = %d\n", getpid());

    int number = atoi(argv[1]);
    createTree(1, number);
}
