
#include <stdio.h>
#include <stdlib.h>
#include "meu_banco.h"

int main() {

    Cliente *clientes = NULL;

    int quantidade = 0;
    int opcao;
    int bancoAberto = 0;

    char sair = 'n';

    do {

        printf("\n========== BANCO DIGITAL ==========\n");
        printf("1 - Abrir banco\n");
        printf("2 - Historico do banco\n");
        printf("3 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);

        switch (opcao) {

            case 1:

                if (bancoAberto == 1) {

                    printf("\nO banco ja esta aberto!\n");

                } else {

                    printf("\nQuantos clientes deseja cadastrar? ");
                    scanf("%d", &quantidade);

                    if (quantidade <= 0) {

                        printf("\nQuantidade de clientes invalida!\n");

                    } else {

                        clientes = malloc(quantidade * sizeof(Cliente));

                        if (clientes == NULL) {

                            printf("\nErro ao alocar memoria!\n");
                            quantidade = 0;

                        } else {

                            cadastrarClientes(clientes, quantidade);

                            bancoAberto = 1;

                            printf("\nBanco aberto com sucesso!\n");
                        }
                    }
                }

                break;

            case 2:

                if (bancoAberto == 1) {

                    exibirRelatorios(clientes, quantidade);

                } else {

                    printf("\nO banco precisa ser aberto primeiro!\n");
                }

                break;

            case 3:

                printf("\nDeseja realmente sair? (s/n): ");
                scanf(" %c", &sair);

                if (sair == 'n' || sair == 'N') {
                    opcao = 0;
                }

                break;

            default:

                printf("\nOpcao invalida! Tente novamente.\n");

                break;
        }

    } while (opcao != 3 || (sair != 's' && sair != 'S'));

    free(clientes);

    printf("\nSistema encerrado. Ate logo!\n");

    return 0;
}
