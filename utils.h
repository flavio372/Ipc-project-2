#ifndef UTILS_H
#define UTILS_H
#define TAM_NOME 50
#define TAM_CAT 30
#define TAM_DESC 60
#define TAM_PAT 20
#define TAM_LOCAL 50
#define TAM_DATA 11
#define TAM_OP 120
void copiar_texto(char *dest, const char *orig, int tam);
int ler_linha(char *dest, int tam);
void ler_texto(const char *msg, char *dest, int tam);
int ler_numero(const char *msg, int *valor);
int pedir_inteiro(const char *msg, int min, int max);
int perguntar_sim_nao(const char *msg);
int data_valida(const char *data);
void ler_data(const char *msg, char *dest);
void pegar_data_atual(char *dest);
int iguais_sem_caixa(const char *a, const char *b);
#endif
