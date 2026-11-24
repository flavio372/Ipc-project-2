#ifndef HISTORICO_H
#define HISTORICO_H
#include "utils.h"
#define MOV_EMPRESTIMO 1
#define MOV_DEVOLUCAO 2
#define MOV_MANUT_ENTRADA 3
#define MOV_MANUT_SAIDA 4
#define MOV_CANCELAMENTO 5
typedef struct {
    int tipo;
    int id_equip;
    int protocolo;
    char nome[TAM_NOME];
    int matricula;
    char data[TAM_DATA];
} Movimentacao;
typedef struct NoHist {
    Movimentacao dados;
    struct NoHist *prox;
} NoHist;
typedef struct {
    NoHist *inicio;
    NoHist *fim;
    int total;
} Historico;
void iniciar_historico(Historico *h);
int inserir_historico(Historico *h, Movimentacao dados);
void listar_historico(Historico *h);
void liberar_historico(Historico *h);
#endif
