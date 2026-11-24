#include <stdio.h>
#include <stdlib.h>
#include "solicitacoes.h"
// deixa a fila vazia (o protocolo comeca em 1)
void iniciar_fila(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->proximo_protocolo = 1;
}
// coloca no final da fila. devolve 1 se deu certo e 0 se faltou memoria
int enfileirar(Fila *fila, Solicitacao dados) {
    NoSolic *novo = (NoSolic *) malloc(sizeof(NoSolic));
    if (novo == NULL) {
        return 0;
    }
    novo->dados = dados;
    novo->prox = NULL;
    if (fila->fim == NULL) {
        fila->inicio = novo;
    } else {
        fila->fim->prox = novo;
    }
    fila->fim = novo;
    fila->tamanho++;
    return 1;
}
// tira o primeiro da fila. devolve 0 se a fila estiver vazia
int desenfileirar(Fila *fila, Solicitacao *removida) {
    NoSolic *aux = fila->inicio;
    if (aux == NULL) {
        return 0;
    }
    *removida = aux->dados;
    fila->inicio = aux->prox;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    free(aux);
    fila->tamanho--;
    return 1;
}
// olha o primeiro da fila sem remover
Solicitacao *primeira_solicitacao(Fila *fila) {
    if (fila->inicio == NULL) {
        return NULL;
    }
    return &fila->inicio->dados;
}
// pega o primeiro no e coloca no final (sem alocar nada novo)
void mover_primeira_para_fim(Fila *fila) {
    NoSolic *aux = fila->inicio;
    if (aux == NULL || aux->prox == NULL) {
        return;
    }
    fila->inicio = aux->prox;
    aux->prox = NULL;
    fila->fim->prox = aux;
    fila->fim = aux;
}
// remove o protocolo informado sem mexer na ordem dos outros. devolve 0 se nao achou
int cancelar_solicitacao(Fila *fila, int protocolo, Solicitacao *removida) {
    NoSolic *atual = fila->inicio;
    NoSolic *anterior = NULL;
    while (atual != NULL && atual->dados.protocolo != protocolo) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) {
        return 0;
    }
    *removida = atual->dados;
    if (anterior == NULL) {
        fila->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    if (atual == fila->fim) {
        fila->fim = anterior;
    }
    free(atual);
    fila->tamanho--;
    return 1;
}
// mostra a fila na ordem de atendimento sem remover ninguem
void listar_fila(Fila *fila) {
    NoSolic *aux;
    int posicao = 1;
    if (fila->inicio == NULL) {
        printf("Fila de solicitacoes vazia.\n");
        return;
    }
    printf("Fila de solicitacoes (%d):\n", fila->tamanho);
    for (aux = fila->inicio; aux != NULL; aux = aux->prox) {
        printf("%do | Protocolo %d | %s (%d) | %s | %s\n", posicao, aux->dados.protocolo, aux->dados.nome, aux->dados.matricula, aux->dados.categoria, aux->dados.local);
        posicao++;
    }
}
// da free em todos os nos da fila
void liberar_fila(Fila *fila) {
    NoSolic *aux;
    while (fila->inicio != NULL) {
        aux = fila->inicio;
        fila->inicio = aux->prox;
        free(aux);
    }
    fila->fim = NULL;
    fila->tamanho = 0;
}
