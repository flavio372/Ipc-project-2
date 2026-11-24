#include <stdio.h>
#include <stdlib.h>
#include "arquivos.h"
// grava a quantidade e depois so os dados de cada equipamento (sem ponteiros)
int salvar_equipamentos(const char *arquivo, NoEquip *inicio) {
    FILE *f = fopen(arquivo, "wb");
    NoEquip *aux;
    int qtd = contar_equipamentos(inicio);
    if (f == NULL) {
        return -1;
    }
    if (fwrite(&qtd, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }
    for (aux = inicio; aux != NULL; aux = aux->prox) {
        if (fwrite(&aux->dados, sizeof(Equipamento), 1, f) != 1) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return qtd;
}
// le o arquivo e recria os nos. se der erro libera o que ja foi criado
int carregar_equipamentos(const char *arquivo, NoEquip **inicio) {
    FILE *f = fopen(arquivo, "rb");
    Equipamento e;
    int qtd, i;
    int erro = 0;
    *inicio = NULL;
    if (f == NULL) {
        return -1;
    }
    if (fread(&qtd, sizeof(int), 1, f) != 1 || qtd < 0) {
        fclose(f);
        return -1;
    }
    for (i = 0; i < qtd && erro == 0; i++) {
        if (fread(&e, sizeof(Equipamento), 1, f) != 1) {
            erro = 1;
        } else {
            // garante o fim dos textos caso o arquivo esteja estragado
            e.descricao[TAM_DESC - 1] = '\0';
            e.categoria[TAM_CAT - 1] = '\0';
            e.patrimonio[TAM_PAT - 1] = '\0';
            e.resp_nome[TAM_NOME - 1] = '\0';
            if (e.id < 1 || e.estado < DISPONIVEL || e.estado > MANUTENCAO || inserir_equipamento(inicio, e) != 1) {
                erro = 1;
            }
        }
    }
    fclose(f);
    if (erro == 1) {
        liberar_equipamentos(inicio);
        return -1;
    }
    return qtd;
}
// grava a quantidade e depois cada movimentacao na ordem cronologica
int salvar_historico(const char *arquivo, Historico *h) {
    FILE *f = fopen(arquivo, "wb");
    NoHist *aux;
    int qtd = h->total;
    if (f == NULL) {
        return -1;
    }
    if (fwrite(&qtd, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }
    for (aux = h->inicio; aux != NULL; aux = aux->prox) {
        if (fwrite(&aux->dados, sizeof(Movimentacao), 1, f) != 1) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return qtd;
}
// le o historico e recria a lista na mesma ordem
int carregar_historico(const char *arquivo, Historico *h) {
    FILE *f = fopen(arquivo, "rb");
    Movimentacao m;
    int qtd, i;
    int erro = 0;
    iniciar_historico(h);
    if (f == NULL) {
        return -1;
    }
    if (fread(&qtd, sizeof(int), 1, f) != 1 || qtd < 0) {
        fclose(f);
        return -1;
    }
    for (i = 0; i < qtd && erro == 0; i++) {
        if (fread(&m, sizeof(Movimentacao), 1, f) != 1) {
            erro = 1;
        } else {
            m.nome[TAM_NOME - 1] = '\0';
            m.data[TAM_DATA - 1] = '\0';
            if (m.tipo < MOV_EMPRESTIMO || m.tipo > MOV_CANCELAMENTO || inserir_historico(h, m) == 0) {
                erro = 1;
            }
        }
    }
    fclose(f);
    if (erro == 1) {
        liberar_historico(h);
        return -1;
    }
    return qtd;
}
// grava o proximo protocolo, a quantidade e as solicitacoes da frente para o fim
int salvar_fila(const char *arquivo, Fila *fila) {
    FILE *f = fopen(arquivo, "wb");
    NoSolic *aux;
    if (f == NULL) {
        return -1;
    }
    if (fwrite(&fila->proximo_protocolo, sizeof(int), 1, f) != 1 || fwrite(&fila->tamanho, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }
    for (aux = fila->inicio; aux != NULL; aux = aux->prox) {
        if (fwrite(&aux->dados, sizeof(Solicitacao), 1, f) != 1) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return fila->tamanho;
}
// le a fila e recria os nos na mesma ordem
int carregar_fila(const char *arquivo, Fila *fila) {
    FILE *f = fopen(arquivo, "rb");
    Solicitacao s;
    int proximo, qtd, i;
    int erro = 0;
    iniciar_fila(fila);
    if (f == NULL) {
        return -1;
    }
    if (fread(&proximo, sizeof(int), 1, f) != 1 || fread(&qtd, sizeof(int), 1, f) != 1 || proximo < 1 || qtd < 0) {
        fclose(f);
        return -1;
    }
    for (i = 0; i < qtd && erro == 0; i++) {
        if (fread(&s, sizeof(Solicitacao), 1, f) != 1) {
            erro = 1;
        } else {
            s.nome[TAM_NOME - 1] = '\0';
            s.categoria[TAM_CAT - 1] = '\0';
            s.local[TAM_LOCAL - 1] = '\0';
            s.data[TAM_DATA - 1] = '\0';
            if (enfileirar(fila, s) == 0) {
                erro = 1;
            }
        }
    }
    fclose(f);
    if (erro == 1) {
        liberar_fila(fila);
        return -1;
    }
    fila->proximo_protocolo = proximo;
    return qtd;
}
// grava a pilha do topo para a base
int salvar_pilha(const char *arquivo, NoOper *topo) {
    FILE *f = fopen(arquivo, "wb");
    NoOper *aux;
    int qtd = contar_operacoes(topo);
    if (f == NULL) {
        return -1;
    }
    if (fwrite(&qtd, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }
    for (aux = topo; aux != NULL; aux = aux->prox) {
        if (fwrite(&aux->dados, sizeof(Operacao), 1, f) != 1) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return qtd;
}
// le a pilha (topo primeiro) e vai ligando cada no no fundo
int carregar_pilha(const char *arquivo, NoOper **topo) {
    FILE *f = fopen(arquivo, "rb");
    Operacao o;
    int qtd, i;
    int erro = 0;
    *topo = NULL;
    if (f == NULL) {
        return -1;
    }
    if (fread(&qtd, sizeof(int), 1, f) != 1 || qtd < 0) {
        fclose(f);
        return -1;
    }
    for (i = 0; i < qtd && erro == 0; i++) {
        if (fread(&o, sizeof(Operacao), 1, f) != 1) {
            erro = 1;
        } else {
            o.descricao[TAM_OP - 1] = '\0';
            o.data[TAM_DATA - 1] = '\0';
            if (inserir_operacao_no_fundo(topo, o) == 0) {
                erro = 1;
            }
        }
    }
    fclose(f);
    if (erro == 1) {
        liberar_operacoes(topo);
        return -1;
    }
    return qtd;
}
