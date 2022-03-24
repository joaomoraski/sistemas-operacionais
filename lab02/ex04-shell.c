//
// Created by moraski on 20/03/2022.
//
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "sys/wait.h"
#include "math.h"

int compare(char command[300], char exitCondition[300]) {
    int len;
    if (strlen(command) > strlen(exitCondition)) len = strlen(command);
    else len = strlen(exitCondition);

    for (int i = 0; i < len; ++i) {
        if (command[i] != exitCondition[i]){
            return 0;
        }
    }
    return 1;
}


void shell() {
    while (1) {
        int childPid;
        char command[300], *user = getenv("USER");
        printf("%s>%s: ", user, user);
        fgets(command, 300, stdin);

        if (compare(command, "exit")) break;

        for (int i = 0; i < strlen(command); ++i) {
            if (command[i] == '&') {
                childPid = fork();
                if (childPid == 0) {
                    system(command);
                    printf("Comando executado em segundo plano: %s\n", command);
                    kill(getpid(), SIGKILL);
                }
            }
        }
        printf("Comando executado: %s\n", command);
        system(command);
    }
}


int main() {
    shell();
}