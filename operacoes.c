#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoes.h"
// coloca uma operacao no topo da pilha. id_equip e protocolo valem 0 quando nao se aplicam
int empilhar_operacao(NoOper **topo, const char *descricao, const char *data, int id_equip, int protocolo) {
    NoOper *novo = (NoOper *) malloc(sizeof(NoOper));
    if (novo == NULL) {
        return 0;
    }
    memset(&novo->dados, 0, sizeof(Operacao));
    if (*topo == NULL) {
        novo->dados.numero = 1;
    } else {
        novo->dados.numero = (*topo)->dados.numero + 1;
    }
    copiar_texto(novo->dados.descricao, descricao, TAM_OP);
    copiar_texto(novo->dados.data, data, TAM_DATA);
    novo->dados.id_equip = id_equip;
    novo->dados.protocolo = protocolo;
    novo->prox = *topo;
    *topo = novo;
    return 1;
}
// usada so no carregamento: coloca a operacao na base da pilha para manter a ordem do arquivo
int inserir_operacao_no_fundo(NoOper **topo, Operacao dados) {
    NoOper *novo = (NoOper *) malloc(sizeof(NoOper));
    NoOper *aux;
    if (novo == NULL) {
        return 0;
    }
    novo->dados = dados;
    novo->prox = NULL;
    if (*topo == NULL) {
        *topo = novo;
        return 1;
    }
    aux = *topo;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;
    return 1;
}
// mostra da operacao mais recente para a mais antiga
void listar_operacoes(NoOper *topo) {
    NoOper *aux;
    if (topo == NULL) {
        printf("Nenhuma operacao registrada.\n");
        return;
    }
    for (aux = topo; aux != NULL; aux = aux->prox) {
        printf("%2d. %s (%s)\n", aux->dados.numero, aux->dados.descricao, aux->dados.data);
    }
}
// conta quantas operacoes tem na pilha
int contar_operacoes(NoOper *topo) {
    int total = 0;
    NoOper *aux;
    for (aux = topo; aux != NULL; aux = aux->prox) {
        total++;
    }
    return total;
}
// da free em toda a pilha
void liberar_operacoes(NoOper **topo) {
    NoOper *aux;
    while (*topo != NULL) {
        aux = *topo;
        *topo = aux->prox;
        free(aux);
    }
}
