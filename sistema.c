#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "arquivos.h"
// deixa tudo vazio no comeco do programa
void iniciar_sistema(Sistema *s) {
    s->equipamentos = NULL;
    iniciar_fila(&s->fila);
    s->pilha = NULL;
    iniciar_historico(&s->historico);
}
// libera toda a memoria alocada
void liberar_sistema(Sistema *s) {
    liberar_equipamentos(&s->equipamentos);
    liberar_fila(&s->fila);
    liberar_operacoes(&s->pilha);
    liberar_historico(&s->historico);
}
// empilha uma operacao que deu certo, usando a data de hoje
void registrar_operacao(Sistema *s, const char *descricao, int id_equip, int protocolo) {
    char data[TAM_DATA];
    pegar_data_atual(data);
    if (empilhar_operacao(&s->pilha, descricao, data, id_equip, protocolo) == 0) {
        printf("Aviso: sem memoria para guardar a operacao na pilha.\n");
    }
}
// guarda uma movimentacao concluida no historico
void registrar_movimentacao(Sistema *s, int tipo, int id_equip, int protocolo, const char *nome, int matricula) {
    Movimentacao m;
    memset(&m, 0, sizeof(Movimentacao));
    m.tipo = tipo;
    m.id_equip = id_equip;
    m.protocolo = protocolo;
    copiar_texto(m.nome, nome, TAM_NOME);
    m.matricula = matricula;
    pegar_data_atual(m.data);
    if (inserir_historico(&s->historico, m) == 0) {
        printf("Aviso: sem memoria para guardar a movimentacao no historico.\n");
    }
}
// opcao 1
void cadastrar_equipamento(Sistema *s) {
    Equipamento e;
    char descricao[TAM_OP];
    int resultado;
    memset(&e, 0, sizeof(Equipamento));
    e.id = pedir_inteiro("Identificador: ", 1, 999999);
    if (buscar_equipamento(s->equipamentos, e.id) != NULL) {
        printf("Erro: ja existe equipamento com identificador %d.\n", e.id);
        return;
    }
    ler_texto("Descricao: ", e.descricao, TAM_DESC);
    ler_texto("Categoria: ", e.categoria, TAM_CAT);
    ler_texto("Codigo de patrimonio: ", e.patrimonio, TAM_PAT);
    e.estado = DISPONIVEL;
    resultado = inserir_equipamento(&s->equipamentos, e);
    if (resultado == 1) {
        printf("Equipamento %d cadastrado.\n", e.id);
        snprintf(descricao, TAM_OP, "Cadastro do equipamento %d", e.id);
        registrar_operacao(s, descricao, e.id, 0);
    } else if (resultado == 0) {
        printf("Erro: ja existe equipamento com identificador %d.\n", e.id);
    } else {
        printf("Erro: sem memoria para cadastrar o equipamento.\n");
    }
}
// opcao 2
void listar_todos_equipamentos(Sistema *s) {
    listar_equipamentos(s->equipamentos);
}
// opcao 3
void buscar_equipamentos(Sistema *s) {
    int tipo;
    int id;
    char categoria[TAM_CAT];
    NoEquip *no;
    if (s->equipamentos == NULL) {
        printf("Erro: nenhum equipamento cadastrado.\n");
        return;
    }
    printf("1 - buscar por identificador\n2 - listar por categoria\n");
    tipo = pedir_inteiro("Escolha: ", 1, 2);
    if (tipo == 1) {
        id = pedir_inteiro("Identificador: ", 1, 999999);
        no = buscar_equipamento(s->equipamentos, id);
        if (no == NULL) {
            printf("Erro: equipamento %d nao encontrado.\n", id);
        } else {
            imprimir_cabecalho_equipamentos();
            imprimir_equipamento(&no->dados);
        }
    } else {
        ler_texto("Categoria: ", categoria, TAM_CAT);
        if (listar_por_categoria(s->equipamentos, categoria) == 0) {
            printf("Nenhum equipamento da categoria %s.\n", categoria);
        }
    }
}
// opcao 4
void registrar_solicitacao(Sistema *s) {
    Solicitacao sol;
    char descricao[TAM_OP];
    memset(&sol, 0, sizeof(Solicitacao));
    ler_texto("Nome do solicitante: ", sol.nome, TAM_NOME);
    sol.matricula = pedir_inteiro("Matricula: ", 1, 99999999);
    ler_texto("Categoria solicitada: ", sol.categoria, TAM_CAT);
    if (solicitante_tem_categoria(s->equipamentos, sol.matricula, sol.categoria)) {
        printf("Erro: %s (%d) ja possui um(a) %s emprestado(a).\n", sol.nome, sol.matricula, sol.categoria);
        return;
    }
    ler_texto("Local de trabalho (frente de lavra, galeria ou nivel): ", sol.local, TAM_LOCAL);
    ler_data("Data (dd/mm/aaaa): ", sol.data);
    sol.protocolo = s->fila.proximo_protocolo;
    if (enfileirar(&s->fila, sol) == 0) {
        printf("Erro: sem memoria para registrar a solicitacao.\n");
        return;
    }
    s->fila.proximo_protocolo++;
    printf("Solicitacao registrada. Protocolo: %d. Posicao na fila: %d.\n", sol.protocolo, s->fila.tamanho);
    snprintf(descricao, TAM_OP, "Solicitacao registrada: protocolo %d (%s)", sol.protocolo, sol.nome);
    registrar_operacao(s, descricao, 0, sol.protocolo);
}
// opcao 5
void listar_solicitacoes(Sistema *s) {
    listar_fila(&s->fila);
}
// opcao 6
void atender_solicitacao(Sistema *s) {
    Solicitacao sol;
    Solicitacao *primeira;
    NoEquip *equip;
    char descricao[TAM_OP];
    int escolha;
    primeira = primeira_solicitacao(&s->fila);
    if (primeira == NULL) {
        printf("Erro: a fila de solicitacoes esta vazia.\n");
        return;
    }
    sol = *primeira; // copia porque o no pode ser removido
    equip = primeiro_disponivel(s->equipamentos, sol.categoria);
    if (equip != NULL) {
        desenfileirar(&s->fila, &sol);
        emprestar_equipamento(equip, sol.nome, sol.matricula);
        printf("Protocolo %d (%s): equipamento %d emprestado.\n", sol.protocolo, sol.nome, equip->dados.id);
        registrar_movimentacao(s, MOV_EMPRESTIMO, equip->dados.id, sol.protocolo, sol.nome, sol.matricula);
        snprintf(descricao, TAM_OP, "Emprestimo do equipamento %d a %s (protocolo %d)", equip->dados.id, sol.nome, sol.protocolo);
        registrar_operacao(s, descricao, equip->dados.id, sol.protocolo);
        return;
    }
    printf("Protocolo %d (%s): nao ha %s disponivel.\n", sol.protocolo, sol.nome, sol.categoria);
    printf("(1) manter no inicio  (2) recolocar no final  (3) cancelar\n");
    escolha = pedir_inteiro("Escolha: ", 1, 3);
    if (escolha == 1) {
        printf("Protocolo %d mantido no inicio da fila.\n", sol.protocolo);
    } else if (escolha == 2) {
        mover_primeira_para_fim(&s->fila);
        printf("Protocolo %d recolocado no final da fila.\n", sol.protocolo);
        snprintf(descricao, TAM_OP, "Protocolo %d recolocado no final da fila", sol.protocolo);
        registrar_operacao(s, descricao, 0, sol.protocolo);
    } else {
        desenfileirar(&s->fila, &sol);
        printf("Protocolo %d cancelado.\n", sol.protocolo);
        registrar_movimentacao(s, MOV_CANCELAMENTO, 0, sol.protocolo, sol.nome, sol.matricula);
        snprintf(descricao, TAM_OP, "Cancelamento do protocolo %d (%s)", sol.protocolo, sol.nome);
        registrar_operacao(s, descricao, 0, sol.protocolo);
    }
}
// opcao 7
void registrar_devolucao(Sistema *s) {
    NoEquip *no;
    char nome[TAM_NOME];
    char descricao[TAM_OP];
    int id, matricula;
    if (s->equipamentos == NULL) {
        printf("Erro: nenhum equipamento cadastrado.\n");
        return;
    }
    id = pedir_inteiro("Identificador do equipamento: ", 1, 999999);
    no = buscar_equipamento(s->equipamentos, id);
    if (no == NULL) {
        printf("Erro: equipamento %d nao encontrado.\n", id);
        return;
    }
    if (no->dados.estado != EMPRESTADO) {
        printf("Erro: equipamento %d nao esta emprestado.\n", id);
        return;
    }
    copiar_texto(nome, no->dados.resp_nome, TAM_NOME); // guarda antes de limpar
    matricula = no->dados.resp_matricula;
    devolver_equipamento(no);
    printf("Devolucao registrada. Equipamento %d disponivel.\n", id);
    registrar_movimentacao(s, MOV_DEVOLUCAO, id, 0, nome, matricula);
    snprintf(descricao, TAM_OP, "Devolucao do equipamento %d (%s)", id, nome);
    registrar_operacao(s, descricao, id, 0);
}
// opcao 8
void alterar_estado_manutencao(Sistema *s) {
    NoEquip *no;
    char descricao[TAM_OP];
    int id, resultado;
    if (s->equipamentos == NULL) {
        printf("Erro: nenhum equipamento cadastrado.\n");
        return;
    }
    id = pedir_inteiro("Identificador do equipamento: ", 1, 999999);
    no = buscar_equipamento(s->equipamentos, id);
    if (no == NULL) {
        printf("Erro: equipamento %d nao encontrado.\n", id);
        return;
    }
    resultado = alterar_manutencao(no);
    if (resultado == 0) {
        printf("Erro: equipamento %d esta emprestado e nao pode entrar em manutencao.\n", id);
    } else if (resultado == 1) {
        printf("Equipamento %d enviado para manutencao.\n", id);
        registrar_movimentacao(s, MOV_MANUT_ENTRADA, id, 0, "", 0);
        snprintf(descricao, TAM_OP, "Equipamento %d enviado para manutencao", id);
        registrar_operacao(s, descricao, id, 0);
    } else {
        printf("Equipamento %d voltou da manutencao e esta disponivel.\n", id);
        registrar_movimentacao(s, MOV_MANUT_SAIDA, id, 0, "", 0);
        snprintf(descricao, TAM_OP, "Equipamento %d voltou da manutencao", id);
        registrar_operacao(s, descricao, id, 0);
    }
}
// opcao 9
void cancelar_solicitacao_fila(Sistema *s) {
    Solicitacao removida;
    char descricao[TAM_OP];
    int protocolo;
    protocolo = pedir_inteiro("Protocolo da solicitacao: ", 1, 999999999);
    if (cancelar_solicitacao(&s->fila, protocolo, &removida) == 0) {
        printf("Erro: protocolo %d nao encontrado na fila.\n", protocolo);
        return;
    }
    printf("Protocolo %d cancelado.\n", protocolo);
    registrar_movimentacao(s, MOV_CANCELAMENTO, 0, protocolo, removida.nome, removida.matricula);
    snprintf(descricao, TAM_OP, "Cancelamento do protocolo %d (%s)", protocolo, removida.nome);
    registrar_operacao(s, descricao, 0, protocolo);
}
// opcao 10
void mostrar_estado_sistema(Sistema *s) {
    printf("=== Equipamentos ===\n");
    listar_equipamentos(s->equipamentos);
    printf("=== Fila de solicitacoes ===\n");
    if (s->fila.inicio == NULL) {
        printf("Fila vazia.\n");
    } else {
        listar_fila(&s->fila);
    }
    printf("=== Operacoes recentes (topo primeiro) ===\n");
    listar_operacoes(s->pilha);
    printf("=== Historico de movimentacoes ===\n");
    listar_historico(&s->historico);
}
// opcao 11
void salvar_dados(Sistema *s) {
    int qtd_equip = salvar_equipamentos(ARQ_EQUIPAMENTOS, s->equipamentos);
    int qtd_hist = salvar_historico(ARQ_HISTORICO, &s->historico);
    int qtd_fila = salvar_fila(ARQ_FILA, &s->fila);
    int qtd_pilha = salvar_pilha(ARQ_PILHA, s->pilha);
    if (qtd_equip < 0 || qtd_hist < 0 || qtd_fila < 0 || qtd_pilha < 0) {
        printf("Erro: nao foi possivel gravar os arquivos.\n");
        return;
    }
    printf("Dados salvos: %d equipamentos, %d movimentacoes, %d solicitacoes, %d operacoes.\n", qtd_equip, qtd_hist, qtd_fila, qtd_pilha);
}
// opcao 12: le tudo para estruturas temporarias e so depois troca pelas atuais
void carregar_dados(Sistema *s) {
    NoEquip *equip = NULL;
    NoOper *pilha = NULL;
    Fila fila;
    Historico hist;
    int qtd_equip, qtd_hist, qtd_fila, qtd_pilha;
    iniciar_fila(&fila);
    iniciar_historico(&hist);
    qtd_equip = carregar_equipamentos(ARQ_EQUIPAMENTOS, &equip);
    qtd_hist = carregar_historico(ARQ_HISTORICO, &hist);
    qtd_fila = carregar_fila(ARQ_FILA, &fila);
    qtd_pilha = carregar_pilha(ARQ_PILHA, &pilha);
    if (qtd_equip < 0 || qtd_hist < 0 || qtd_fila < 0 || qtd_pilha < 0) {
        printf("Erro: nao foi possivel carregar os dados (arquivo ausente ou corrompido). Os dados atuais foram mantidos.\n");
        liberar_equipamentos(&equip);
        liberar_historico(&hist);
        liberar_fila(&fila);
        liberar_operacoes(&pilha);
        return;
    }
    liberar_sistema(s); // libera o que estava na memoria antes de usar os dados do arquivo
    s->equipamentos = equip;
    s->fila = fila;
    s->pilha = pilha;
    s->historico = hist;
    printf("Dados carregados: %d equipamentos, %d movimentacoes, %d solicitacoes, %d operacoes.\n", qtd_equip, qtd_hist, qtd_fila, qtd_pilha);
}
