#include <stdio.h>
#include "sistema.h"
// mostra as opcoes do menu
void mostrar_menu(void) {
    printf("\n===== LAMPISTERIA =====\n");
    printf(" 1 - Cadastrar equipamento\n");
    printf(" 2 - Listar equipamentos\n");
    printf(" 3 - Buscar equipamento\n");
    printf(" 4 - Registrar solicitacao\n");
    printf(" 5 - Listar solicitacoes pendentes\n");
    printf(" 6 - Atender proxima solicitacao\n");
    printf(" 7 - Registrar devolucao\n");
    printf(" 8 - Alterar manutencao\n");
    printf(" 9 - Cancelar solicitacao\n");
    printf("10 - Mostrar estado do sistema\n");
    printf("11 - Salvar dados\n");
    printf("12 - Carregar dados\n");
    printf(" 0 - Sair\n");
}
int main(void) {
    Sistema sistema;
    int opcao = -1;
    iniciar_sistema(&sistema);
    do {
        mostrar_menu();
        if (ler_numero("Escolha uma opcao: ", &opcao) == 0) {
            printf("Erro: entrada invalida, digite um numero de 0 a 12.\n");
            opcao = -1;
            continue;
        }
        switch (opcao) {
            case 1: cadastrar_equipamento(&sistema); break;
            case 2: listar_todos_equipamentos(&sistema); break;
            case 3: buscar_equipamentos(&sistema); break;
            case 4: registrar_solicitacao(&sistema); break;
            case 5: listar_solicitacoes(&sistema); break;
            case 6: atender_solicitacao(&sistema); break;
            case 7: registrar_devolucao(&sistema); break;
            case 8: alterar_estado_manutencao(&sistema); break;
            case 9: cancelar_solicitacao_fila(&sistema); break;
            case 10: mostrar_estado_sistema(&sistema); break;
            case 11: salvar_dados(&sistema); break;
            case 12: carregar_dados(&sistema); break;
            case 0:
                if (perguntar_sim_nao("Deseja salvar antes de sair? (s/n): ") == 1) {
                    salvar_dados(&sistema);
                }
                liberar_sistema(&sistema);
                printf("Memoria liberada. Encerrando o sistema.\n");
                break;
            default:
                printf("Erro: a opcao %d nao existe, escolha um numero de 0 a 12.\n", opcao);
        }
    } while (opcao != 0);
    return 0;
}
