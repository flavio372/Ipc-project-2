#ifndef OPERACOES_H
#define OPERACOES_H
#include "utils.h"
typedef struct {
    int numero;
    char descricao[TAM_OP];
    char data[TAM_DATA];
    int id_equip;
    int protocolo;
} Operacao;
typedef struct NoOper {
    Operacao dados;
    struct NoOper *prox;
} NoOper;
int empilhar_operacao(NoOper **topo, const char *descricao, const char *data, int id_equip, int protocolo);
int inserir_operacao_no_fundo(NoOper **topo, Operacao dados);
void listar_operacoes(NoOper *topo);
int contar_operacoes(NoOper *topo);
void liberar_operacoes(NoOper **topo);
#endif
