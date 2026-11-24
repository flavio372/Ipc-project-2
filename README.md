# Controle da Lampisteria

Trabalho 2 de Introdução à Programação de Computadores (CEFET-MG Divinópolis, Engenharia de Computação).
Sistema em C para controlar o empréstimo de lanternas, autorresgatadores e outros equipamentos da mina.

## Como compilar e executar (Linux)

```
make
./lampisteria
```

ou, sem o Makefile:

```
gcc -Wall -Wextra -o lampisteria main.c sistema.c equipamentos.c solicitacoes.c operacoes.c historico.c arquivos.c utils.c
```

## Organização dos arquivos

| arquivo | responsabilidade |
|---|---|
| main.c | só o menu em loop e a chamada das funções |
| sistema.c/.h | junta as estruturas e implementa cada opção do menu |
| equipamentos.c/.h | lista encadeada de equipamentos (ordenada por id) |
| solicitacoes.c/.h | fila dinâmica de solicitações |
| operacoes.c/.h | pilha dinâmica de operações recentes |
| historico.c/.h | lista encadeada do histórico de movimentações |
| arquivos.c/.h | salvar e carregar arquivos binários |
| utils.c/.h | leitura segura de textos e números, validação de data |

## Estruturas de dados

- **Equipamentos:** lista simplesmente encadeada, inserção sempre na posição certa para manter a ordem crescente de id. Id repetido é recusado.
- **Solicitações:** fila com ponteiros para início e fim. O protocolo é sequencial e começa em 1. O contador de protocolo fica guardado dentro da struct da fila.
- **Operações recentes:** pilha, a mais recente fica no topo. Cada operação tem um número sequencial para a listagem.
- **Histórico:** lista encadeada com ponteiro para o fim, para inserir em ordem cronológica.

Cada struct de dados (`Equipamento`, `Solicitacao`, `Operacao`, `Movimentacao`) é separada do nó que a guarda na lista. Assim só os dados vão para o arquivo, sem ponteiros.

## Decisões de projeto

- Cadastro e solicitação perguntam um campo por vez (em vez de tudo na mesma linha).
- A categoria é comparada sem diferenciar maiúscula de minúscula ("lanterna" = "Lanterna").
- Ao atender, o programa olha a primeira solicitação sem remover. Só remove se achar equipamento disponível, ou se o usuário escolher cancelar. "Manter no início" não mexe na fila e "recolocar no final" move o nó para o fim, mantendo o mesmo protocolo.
- Só operações que deram certo vão para a pilha. Tentativas recusadas não são empilhadas.
- A data das operações e do histórico é a data do computador. A data da solicitação é digitada pelo usuário.
- Além de equipamentos e histórico (obrigatórios), também salvo a fila e a pilha (melhoria). São 4 arquivos: `equipamentos.bin`, `historico.bin`, `fila.bin`, `pilha.bin`.
- Ao carregar, os dados são lidos para estruturas temporárias. Só se tudo der certo a memória atual é liberada e trocada pelos dados do arquivo. Assim, se um arquivo estiver faltando ou corrompido, os dados que estavam na memória não se perdem.
- Ao sair, o programa pergunta se quer salvar, libera toda a memória e encerra.

## Validações

- opção de menu que não é número ou que não existe (ex.: 99)
- identificador, matrícula, protocolo e escolhas fora do intervalo ou com letras
- campos de texto vazios ou grandes demais
- data fora do formato dd/mm/aaaa ou que não existe (ex.: 31/02/2026)
- identificador repetido, equipamento inexistente, devolução de equipamento que não está emprestado, manutenção de equipamento emprestado
- fila vazia, protocolo inexistente, categoria sem equipamento disponível
- falha ao abrir, ler ou gravar arquivo
- fim da entrada (Ctrl+D) encerra o programa sem ficar em loop

## Testes realizados

Usei a sessão dos exemplos 1 a 6 do enunciado, na ordem, e conferi as saídas:

1. cadastro de 102, 101 e 103; recusa do id 101 repetido; listagem em ordem de id
2. três solicitações (protocolos 1, 2 e 3) e listagem da fila
3. atendimento: 101 e 102 emprestados, protocolo 3 sem Detector de gás e recolocado no final
4. devolução inválida, manutenção inválida, solicitação recusada (já possui a categoria), devolução, manutenção, id inexistente e cancelamento duplo
5. estado do sistema: pilha com 12 operações e histórico com 5 movimentações
6. salvar, sair, abrir de novo, carregar e listar (mesmo estado de antes)

Testes extras: opção `99`, opção `abc`, data `31/02/2026`, escolha `x` no menu de indisponibilidade, entrada acabando no meio de um cadastro. Também compilei com `-fsanitize=address,undefined` e rodei a sessão completa sem erros nem vazamentos.

## Limitações conhecidas

- A mesma pessoa pode ter duas solicitações da mesma categoria na fila ao mesmo tempo (a regra só olha equipamentos já emprestados).
- Matrícula é guardada como número inteiro (até 8 dígitos).
- Se a entrada terminar no meio de uma operação (Ctrl+D), o programa encerra sem perguntar se quer salvar.
