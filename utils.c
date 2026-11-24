#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include "utils.h"
// copia um texto para outro sem estourar o tamanho do destino
void copiar_texto(char *dest, const char *orig, int tam) {
    int i = 0;
    while (orig[i] != '\0' && i < tam - 1) {
        dest[i] = orig[i];
        i++;
    }
    dest[i] = '\0';
}
// le uma linha com fgets e tira o \n. devolve 0 se acabou a entrada, 1 se ok e 2 se o texto era grande demais
int ler_linha(char *dest, int tam) {
    int c;
    int tamanho;
    if (fgets(dest, tam, stdin) == NULL) {
        return 0;
    }
    tamanho = strlen(dest);
    if (tamanho > 0 && dest[tamanho - 1] == '\n') {
        dest[tamanho - 1] = '\0';
        return 1;
    }
    if (tamanho < tam - 1) {
        return 1; // ultima linha do arquivo sem enter
    }
    c = getchar();
    if (c == '\n' || c == EOF) {
        return 1; // coube certinho
    }
    while (c != '\n' && c != EOF) {
        c = getchar(); // joga fora o resto da linha
    }
    return 2;
}
// pede um texto e so aceita se nao for vazio e couber no vetor
void ler_texto(const char *msg, char *dest, int tam) {
    int resultado;
    int i;
    int vazio;
    while (1) {
        printf("%s", msg);
        resultado = ler_linha(dest, tam);
        if (resultado == 0) {
            printf("\nFim da entrada. Encerrando o programa.\n");
            exit(0);
        }
        if (resultado == 2) {
            printf("Erro: texto muito grande (maximo %d caracteres).\n", tam - 1);
            continue;
        }
        vazio = 1;
        for (i = 0; dest[i] != '\0'; i++) {
            if (dest[i] != ' ' && dest[i] != '\t') {
                vazio = 0;
            }
        }
        if (vazio == 1) {
            printf("Erro: o campo nao pode ficar vazio.\n");
            continue;
        }
        return;
    }
}
// le um numero inteiro. devolve 1 se leu certo e 0 se a entrada nao era um numero
int ler_numero(const char *msg, int *valor) {
    char texto[30];
    char *fim;
    long numero;
    int resultado;
    printf("%s", msg);
    resultado = ler_linha(texto, 30);
    if (resultado == 0) {
        printf("\nFim da entrada. Encerrando o programa.\n");
        exit(0);
    }
    if (resultado == 2) {
        return 0;
    }
    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (fim == texto || errno == ERANGE) {
        return 0;
    }
    while (*fim == ' ' || *fim == '\t') {
        fim++;
    }
    if (*fim != '\0' || numero > INT_MAX || numero < INT_MIN) {
        return 0;
    }
    *valor = (int) numero;
    return 1;
}
// repete a pergunta ate o usuario digitar um inteiro entre min e max
int pedir_inteiro(const char *msg, int min, int max) {
    int valor;
    while (1) {
        if (ler_numero(msg, &valor) == 0) {
            printf("Erro: digite apenas um numero inteiro.\n");
        } else if (valor < min || valor > max) {
            printf("Erro: digite um valor entre %d e %d.\n", min, max);
        } else {
            return valor;
        }
    }
}
// pergunta s ou n. devolve 1 para sim e 0 para nao
int perguntar_sim_nao(const char *msg) {
    char texto[10];
    int resultado;
    while (1) {
        printf("%s", msg);
        resultado = ler_linha(texto, 10);
        if (resultado == 0) {
            return 0;
        }
        if (resultado == 1 && texto[1] == '\0') {
            if (texto[0] == 's' || texto[0] == 'S') {
                return 1;
            }
            if (texto[0] == 'n' || texto[0] == 'N') {
                return 0;
            }
        }
        printf("Erro: responda apenas com s ou n.\n");
    }
}
// confere se a data esta no formato dd/mm/aaaa e se existe no calendario
int data_valida(const char *data) {
    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dia, mes, ano, max_dia, i;
    if (strlen(data) != 10) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') {
                return 0;
            }
        } else if (!isdigit((unsigned char) data[i])) {
            return 0;
        }
    }
    dia = atoi(data);
    mes = atoi(data + 3);
    ano = atoi(data + 6);
    if (mes < 1 || mes > 12 || ano < 2000 || ano > 2100) {
        return 0;
    }
    max_dia = dias_mes[mes - 1];
    if (mes == 2 && ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)) {
        max_dia = 29; // ano bissexto
    }
    if (dia < 1 || dia > max_dia) {
        return 0;
    }
    return 1;
}
// pede a data ate o usuario digitar uma valida
void ler_data(const char *msg, char *dest) {
    while (1) {
        ler_texto(msg, dest, TAM_DATA);
        if (data_valida(dest)) {
            return;
        }
        printf("Erro: data invalida, use o formato dd/mm/aaaa.\n");
    }
}
// pega a data do computador no formato dd/mm/aaaa
void pegar_data_atual(char *dest) {
    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    strftime(dest, TAM_DATA, "%d/%m/%Y", info);
}
// compara dois textos sem diferenciar maiuscula de minuscula
int iguais_sem_caixa(const char *a, const char *b) {
    return strcasecmp(a, b) == 0;
}
