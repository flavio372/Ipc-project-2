#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipamentos.h"
// devolve o texto do estado
const char *nome_estado(int estado) {
    if (estado == DISPONIVEL) {
        return "Disponivel";
    }
    if (estado == EMPRESTADO) {
        return "Emprestado";
    }
    return "Manutencao";
}
// insere na posicao certa (ordem crescente de id). devolve 1 se deu certo, 0 se o id ja existe e -1 se faltou memoria
int inserir_equipamento(NoEquip **inicio, Equipamento dados) {
    NoEquip *novo;
    NoEquip *atual = *inicio;
    NoEquip *anterior = NULL;
    while (atual != NULL && atual->dados.id < dados.id) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual != NULL && atual->dados.id == dados.id) {
        return 0;
    }
    novo = (NoEquip *) malloc(sizeof(NoEquip));
    if (novo == NULL) {
        return -1;
    }
    novo->dados = dados;
    novo->prox = atual;
    if (anterior == NULL) {
        *inicio = novo;
    } else {
        anterior->prox = novo;
    }
    return 1;
}
// procura pelo id, devolve NULL se nao achar
NoEquip *buscar_equipamento(NoEquip *inicio, int id) {
    NoEquip *aux = inicio;
    while (aux != NULL && aux->dados.id < id) {
        aux = aux->prox;
    }
    if (aux != NULL && aux->dados.id == id) {
        return aux;
    }
    return NULL;
}
// imprime a linha de titulo da tabela
void imprimir_cabecalho_equipamentos(void) {
    printf("%-5s %-28s %-17s %-11s %-11s %s\n", "ID", "Descricao", "Categoria", "Patrimonio", "Estado", "Responsavel");
}
// imprime uma linha da tabela
void imprimir_equipamento(Equipamento *e) {
    printf("%-5d %-28s %-17s %-11s %-11s ", e->id, e->descricao, e->categoria, e->patrimonio, nome_estado(e->estado));
    if (e->estado == EMPRESTADO) {
        printf("%s (%d)\n", e->resp_nome, e->resp_matricula);
    } else {
        printf("-\n");
    }
}
// mostra todos os equipamentos
void listar_equipamentos(NoEquip *inicio) {
    NoEquip *aux;
    if (inicio == NULL) {
        printf("Nenhum equipamento cadastrado.\n");
        return;
    }
    imprimir_cabecalho_equipamentos();
    for (aux = inicio; aux != NULL; aux = aux->prox) {
        imprimir_equipamento(&aux->dados);
    }
}
// mostra so os de uma categoria e devolve quantos achou
int listar_por_categoria(NoEquip *inicio, const char *categoria) {
    NoEquip *aux;
    int achados = 0;
    for (aux = inicio; aux != NULL; aux = aux->prox) {
        if (iguais_sem_caixa(aux->dados.categoria, categoria)) {
            if (achados == 0) {
                imprimir_cabecalho_equipamentos();
            }
            imprimir_equipamento(&aux->dados);
            achados++;
        }
    }
    return achados;
}
// acha o primeiro disponivel da categoria, andando na ordem de id
NoEquip *primeiro_disponivel(NoEquip *inicio, const char *categoria) {
    NoEquip *aux;
    for (aux = inicio; aux != NULL; aux = aux->prox) {
        if (aux->dados.estado == DISPONIVEL && iguais_sem_caixa(aux->dados.categoria, categoria)) {
            return aux;
        }
    }
    return NULL;
}
// confere se a matricula ja tem um equipamento emprestado daquela categoria
int solicitante_tem_categoria(NoEquip *inicio, int matricula, const char *categoria) {
    NoEquip *aux;
    for (aux = inicio; aux != NULL; aux = aux->prox) {
        if (aux->dados.estado == EMPRESTADO && aux->dados.resp_matricula == matricula && iguais_sem_caixa(aux->dados.categoria, categoria)) {
            return 1;
        }
    }
    return 0;
}
// marca o equipamento como emprestado e guarda o responsavel
void emprestar_equipamento(NoEquip *no, const char *nome, int matricula) {
    no->dados.estado = EMPRESTADO;
    copiar_texto(no->dados.resp_nome, nome, TAM_NOME);
    no->dados.resp_matricula = matricula;
}
// deixa o equipamento disponivel e limpa o responsavel
void devolver_equipamento(NoEquip *no) {
    no->dados.estado = DISPONIVEL;
    no->dados.resp_nome[0] = '\0';
    no->dados.resp_matricula = 0;
}
// troca entre disponivel e manutencao. devolve 0 se esta emprestado, 1 se entrou em manutencao e 2 se saiu
int alterar_manutencao(NoEquip *no) {
    if (no->dados.estado == EMPRESTADO) {
        return 0;
    }
    if (no->dados.estado == DISPONIVEL) {
        no->dados.estado = MANUTENCAO;
        return 1;
    }
    no->dados.estado = DISPONIVEL;
    return 2;
}
// conta os nos da lista
int contar_equipamentos(NoEquip *inicio) {
    int total = 0;
    NoEquip *aux;
    for (aux = inicio; aux != NULL; aux = aux->prox) {
        total++;
    }
    return total;
}
// da free em todos os nos
void liberar_equipamentos(NoEquip **inicio) {
    NoEquip *aux;
    while (*inicio != NULL) {
        aux = *inicio;
        *inicio = aux->prox;
        free(aux);
    }
}
