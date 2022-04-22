// Funcionalidade: Recebe do usuário uma frase que esta contina no Frase.txt e pede qual funcionalidade o programa deve executar
// se é codificar ou decodificar, o algoritmo implementa a logica da Cifra de cesar, que consiste em uma letra ser deslocada para 
// 3 letras a frente do alfabeto, ele fica em loop infinito podendo codificar milhares de vezes e decodificar de volta.
// Autores: João Vitor Moraski Lunkes, Flavio Bernaski, Caio Luiz dos Santos
// Data: 22/04/2022

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

volatile sig_atomic_t graceStop = 1;

// Alfabeto das letras em maiusculo em hexadecimal
char alfabetoMaisculo[26] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
                             0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a};
// Alfabeto das letras em minusculo em hexadecimal
char alfabetoMinusculo[26] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
                              0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a};

// Codificador da cifra de cesar
// Codigo da cifra de Cesar para codificar a frase, recebendo a palavra como parametro
char *cesarCipherEncode(char palavra[40]) {
    int cesarIndex = 0;
    // for para percorrer a palavra
    for (int i = 0; i < strlen(palavra); ++i) {
        // for para percorrer o alfabeto usando a palavra para comparar
        for (int j = 0; j < 26; ++j) {
            // Se a letra estiver no maisculo ele coloca uma letra maiscula com 3 posicoes a frente no lugar
            if (palavra[i] == alfabetoMaisculo[j]) {
                // logica da cifra
                cesarIndex = j + 3;
                // if para que se passe do tamanho do alfabeto volte ao inicio
                if (cesarIndex > 25) cesarIndex -= 26;
                palavra[i] = alfabetoMaisculo[cesarIndex];
                break;
            }
            // Se a letra estiver no minusculo ele coloca uma letra minusculo com 3 posicoes a frente no lugar
            if (palavra[i] == alfabetoMinusculo[j]) {
                // logica da cifra
                cesarIndex = j + 3;
                // if para que se passe do tamanho do alfabeto volte ao inicio
                if (cesarIndex > 25) cesarIndex -= 26;
                palavra[i] = alfabetoMinusculo[cesarIndex];
                break;
            }
        }
    }
    return palavra;
}

// Decodificador da cifra de cesar
// Codigo da cifra de Cesar para decodificar a frase, recebendo a palavra como parametro
char *cesarCipherDecode(char palavra[40]) {
    int cesarIndex = 0;
    // for para percorrer a palavra
    for (int i = 0; i < strlen(palavra); ++i) {
        // for para percorrer o alfabeto usando a palavra para comparar
        for (int j = 0; j < 26; ++j) {
            // Se a letra estiver no maisculo ele coloca uma letra maiscula com 3 posicoes a frente no lugar
            if (palavra[i] == alfabetoMaisculo[j]) {
                // logica da cifra para decodificar  (que seria voltar de 3 posicoes a frente para tras)
                cesarIndex = j - 3;
                if (cesarIndex < 0) cesarIndex += 26;
                palavra[i] = alfabetoMaisculo[cesarIndex];
                break;
            }
            if (palavra[i] == alfabetoMinusculo[j]) {
                // logica da cifra para decodificar  (que seria voltar de 3 posicoes a frente para tras)
                cesarIndex = j - 3;
                // if para que se passe do tamanho do alfabeto volte ao final
                if (cesarIndex < 0) cesarIndex += 26;
                palavra[i] = alfabetoMinusculo[cesarIndex];
                break;
            }
        }
    }
    return palavra;
}


void writeFile(char saida[1024], int n) {
    FILE *file;
    file = fopen("frase.txt", "w");
    fwrite(saida, n, 1, file);
    fclose(file);
}

/* função tratadora de sinais. */
void sig_handler(int sig) {
    if (sig == SIGINT)printf("\nreceived SIGINT\n");

    if (sig == SIGTERM) printf("\nreceived SIGTERM\n");

    printf("Graceful stop\n");
    graceStop = 0;
}

int main(void) {
    /* Associa a função tratadora de sinais */
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGTERM\n");

    /* exibe o PID */
    printf("My PID is %d.\n", getpid());


    // Manipulacao de arquivos
    // Variaveis iniciais
    int opcao;
    char entry[1024], exit[1024] = "";


    // Abre o arquivo e recebe a frase
    FILE *arq;
    arq = fopen("frase.txt", "r");
    fscanf(arq, " %[^\n]", entry);
    fclose(arq);

    while (graceStop) {
        // Pede a opcao para o usuario
        printf("Codificar(0) ou Decodificar(1): ");
        scanf("%d", &opcao);
        if (opcao == 0) {
            // se for 0, codifica a mensagem e salva no arquivo
            printf("Mensagem decodificada: %s\n", entry);
            strcpy(exit, cesarCipherEncode(entry));
            printf("Mensagem codificada obtida: %s\n", entry);
        } else if (opcao == 1) {
            // se for 1, decodifica a mensagem e salva no arquivo
            printf("Mensagem codificada: %s\n", entry);
            strcpy(exit, cesarCipherDecode(entry));
            printf("Mensagem decodificada obtida: %s\n", entry);
        }
        sleep(1);
    }

    // escreve a saida no arquivo(mesmo de entrada)
    writeFile(exit, strlen(exit));

    // mata o processo
    kill(getpid(), SIGKILL);
    return 0;
}