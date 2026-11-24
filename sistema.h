#ifndef SISTEMA_H
#define SISTEMA_H
#include "equipamentos.h"
#include "solicitacoes.h"
#include "operacoes.h"
#include "historico.h"
// junta todas as estruturas do programa num lugar so
typedef struct {
    NoEquip *equipamentos;
    Fila fila;
    NoOper *pilha;
    Historico historico;
} Sistema;
void iniciar_sistema(Sistema *s);
void liberar_sistema(Sistema *s);
void cadastrar_equipamento(Sistema *s);
void listar_todos_equipamentos(Sistema *s);
void buscar_equipamentos(Sistema *s);
void registrar_solicitacao(Sistema *s);
void listar_solicitacoes(Sistema *s);
void atender_solicitacao(Sistema *s);
void registrar_devolucao(Sistema *s);
void alterar_estado_manutencao(Sistema *s);
void cancelar_solicitacao_fila(Sistema *s);
void mostrar_estado_sistema(Sistema *s);
void salvar_dados(Sistema *s);
void carregar_dados(Sistema *s);
#endif
