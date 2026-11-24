#include <stdio.h>
#include <stdlib.h>
#include "historico.h"
// deixa o historico vazio
void iniciar_historico(Historico *h) {
    h->inicio = NULL;
    h->fim = NULL;
    h->total = 0;
}
// insere no final para manter a ordem cronologica. devolve 0 se faltou memoria
int inserir_historico(Historico *h, Movimentacao dados) {
    NoHist *novo = (NoHist *) malloc(sizeof(NoHist));
    if (novo == NULL) {
        return 0;
    }
    novo->dados = dados;
    novo->prox = NULL;
    if (h->fim == NULL) {
        h->inicio = novo;
    } else {
        h->fim->prox = novo;
    }
    h->fim = novo;
    h->total++;
    return 1;
}
// mostra as movimentacoes da mais antiga para a mais nova
void listar_historico(Historico *h) {
    NoHist *aux;
    int numero = 1;
    if (h->inicio == NULL) {
        printf("Nenhuma movimentacao registrada.\n");
        return;
    }
    for (aux = h->inicio; aux != NULL; aux = aux->prox) {
        printf("%d. ", numero);
        if (aux->dados.tipo == MOV_EMPRESTIMO) {
            printf("Emprestimo | equipamento %d | %s (%d) | protocolo %d", aux->dados.id_equip, aux->dados.nome, aux->dados.matricula, aux->dados.protocolo);
        } else if (aux->dados.tipo == MOV_DEVOLUCAO) {
            printf("Devolucao | equipamento %d | %s (%d)", aux->dados.id_equip, aux->dados.nome, aux->dados.matricula);
        } else if (aux->dados.tipo == MOV_MANUT_ENTRADA) {
            printf("Manutencao | equipamento %d | entrada", aux->dados.id_equip);
        } else if (aux->dados.tipo == MOV_MANUT_SAIDA) {
            printf("Manutencao | equipamento %d | saida", aux->dados.id_equip);
        } else {
            printf("Cancelamento | protocolo %d | %s (%d)", aux->dados.protocolo, aux->dados.nome, aux->dados.matricula);
        }
        printf(" | %s\n", aux->dados.data);
        numero++;
    }
}
// da free em todos os nos
void liberar_historico(Historico *h) {
    NoHist *aux;
    while (h->inicio != NULL) {
        aux = h->inicio;
        h->inicio = aux->prox;
        free(aux);
    }
    h->fim = NULL;
    h->total = 0;
}
