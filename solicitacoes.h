#ifndef SOLICITACOES_H
#define SOLICITACOES_H
#include "utils.h"
typedef struct {
    int protocolo;
    char nome[TAM_NOME];
    int matricula;
    char categoria[TAM_CAT];
    char local[TAM_LOCAL];
    char data[TAM_DATA];
} Solicitacao;
typedef struct NoSolic {
    Solicitacao dados;
    struct NoSolic *prox;
} NoSolic;
// a fila guarda o inicio, o fim, o tamanho e o proximo protocolo a ser gerado
typedef struct {
    NoSolic *inicio;
    NoSolic *fim;
    int tamanho;
    int proximo_protocolo;
} Fila;
void iniciar_fila(Fila *fila);
int enfileirar(Fila *fila, Solicitacao dados);
int desenfileirar(Fila *fila, Solicitacao *removida);
Solicitacao *primeira_solicitacao(Fila *fila);
void mover_primeira_para_fim(Fila *fila);
int cancelar_solicitacao(Fila *fila, int protocolo, Solicitacao *removida);
void listar_fila(Fila *fila);
void liberar_fila(Fila *fila);
#endif
