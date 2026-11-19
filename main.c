
#include <stdio.h>
#include <stdlib.h>
#include "meu_banco.h"

int main() {

    Cliente *clientes = NULL;
    int quantidade = 0;
    int opcao;
    int bancoAberto = 0;

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

                    if (quantidade > 0) {

                        clientes = malloc(quantidade * sizeof(Cliente));

                        if (clientes != NULL) {

                            cadastrarClientes(clientes, quantidade);
                            bancoAberto = 1;

                            printf("\nBanco aberto com sucesso!\n");

                        } else {

                            printf("\nErro ao alocar memoria!\n");
                            quantidade = 0;
                        }

                    } else {

                        printf("\nQuantidade invalida!\n");
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

                printf("\nEncerrando o banco...\n");
                break;

            default:

                printf("\nOpcao invalida!\n");
                break;
        }

    } while (opcao != 3);

    free(clientes);

    printf("\nSistema encerrado!\n");

    return 0;
}
