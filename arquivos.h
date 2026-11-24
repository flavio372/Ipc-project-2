#ifndef ARQUIVOS_H
#define ARQUIVOS_H
#include "equipamentos.h"
#include "solicitacoes.h"
#include "operacoes.h"
#include "historico.h"
#define ARQ_EQUIPAMENTOS "equipamentos.bin"
#define ARQ_HISTORICO "historico.bin"
#define ARQ_FILA "fila.bin"
#define ARQ_PILHA "pilha.bin"
// todas as funcoes de salvar devolvem a quantidade gravada (ou -1 se deu erro)
// todas as funcoes de carregar devolvem a quantidade lida (ou -1 se deu erro)
int salvar_equipamentos(const char *arquivo, NoEquip *inicio);
int carregar_equipamentos(const char *arquivo, NoEquip **inicio);
int salvar_historico(const char *arquivo, Historico *h);
int carregar_historico(const char *arquivo, Historico *h);
int salvar_fila(const char *arquivo, Fila *fila);
int carregar_fila(const char *arquivo, Fila *fila);
int salvar_pilha(const char *arquivo, NoOper *topo);
int carregar_pilha(const char *arquivo, NoOper **topo);
#endif
