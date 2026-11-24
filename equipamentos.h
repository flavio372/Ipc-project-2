#ifndef EQUIPAMENTOS_H
#define EQUIPAMENTOS_H
#include "utils.h"
#define DISPONIVEL 0
#define EMPRESTADO 1
#define MANUTENCAO 2
// dados de um equipamento (e so isso que vai pro arquivo)
typedef struct {
    int id;
    char descricao[TAM_DESC];
    char categoria[TAM_CAT];
    char patrimonio[TAM_PAT];
    int estado;
    char resp_nome[TAM_NOME];
    int resp_matricula;
} Equipamento;
// no da lista encadeada
typedef struct NoEquip {
    Equipamento dados;
    struct NoEquip *prox;
} NoEquip;
const char *nome_estado(int estado);
int inserir_equipamento(NoEquip **inicio, Equipamento dados);
NoEquip *buscar_equipamento(NoEquip *inicio, int id);
void imprimir_cabecalho_equipamentos(void);
void imprimir_equipamento(Equipamento *e);
void listar_equipamentos(NoEquip *inicio);
int listar_por_categoria(NoEquip *inicio, const char *categoria);
NoEquip *primeiro_disponivel(NoEquip *inicio, const char *categoria);
int solicitante_tem_categoria(NoEquip *inicio, int matricula, const char *categoria);
void emprestar_equipamento(NoEquip *no, const char *nome, int matricula);
void devolver_equipamento(NoEquip *no);
int alterar_manutencao(NoEquip *no);
int contar_equipamentos(NoEquip *inicio);
void liberar_equipamentos(NoEquip **inicio);
#endif
