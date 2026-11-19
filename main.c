
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "meu_banco.h"

int main() {

    Cliente *clientes = NULL;

    int quantidadeClientes = 0;

    int bancoAberto = 0;
    int sair = 0;

    char opcao;
    char numeroConta[TAM_NUMERO_CONTA];

    Cliente *clienteEncontrado;
    Conta *contaEncontrada;

    srand(time(NULL));

    do {

        printf("\n========== BANCO DIGITAL ==========\n");
        printf("1 - Abrir banco\n");
        printf("2 - Consultar historico\n");
        printf("3 - Efetuar sorteio\n");
        printf("4 - Acessar conta\n");
        printf("s - Sair\n");
        printf("Escolha uma opcao: ");

        scanf(" %c", &opcao);

        switch (opcao) {

            case '1':

                if (bancoAberto == 0) {

                    printf("\nInforme a quantidade de clientes: ");
                    scanf("%d", &quantidadeClientes);

                    if (quantidadeClientes <= 0) {

                        printf("\nQuantidade de clientes invalida!\n");
                        quantidadeClientes = 0;

                    } else {

                        clientes = malloc(quantidadeClientes * sizeof(Cliente));

                        if (clientes == NULL) {

                            printf("\nErro ao alocar memoria!\n");
                            quantidadeClientes = 0;

                        } else {

                            cadastrarClientes(clientes, quantidadeClientes);

                            bancoAberto = 1;

                            printf("\nBanco aberto com sucesso!\n");

                        }
                    }

                } else {

                    printf("\nO banco ja esta aberto!\n");

                }

                break;

            case '2':

                if (bancoAberto == 1) {

                    exibirRelatorios(clientes, quantidadeClientes);

                } else {

                    printf("\nAbra o banco primeiro!\n");

                }

                break;

            case '3':

                if (bancoAberto == 1) {

                    realizarSorteio(clientes, quantidadeClientes);

                } else {

                    printf("\nAbra o banco primeiro!\n");

                }

                break;

            case '4':

                if (bancoAberto == 1) {

                    printf("\n========== ACESSO A CONTA ==========\n");
                    printf("Informe o numero da conta (ex: 1000-01): ");

                    scanf(" %7s", numeroConta);

                    if (buscarConta(
                        clientes,
                        quantidadeClientes,
                        numeroConta,
                        &clienteEncontrado,
                        &contaEncontrada
                    )) {

                        alertarSaldoNegativo(clienteEncontrado);

                        char opcaoCliente;

                        do {

                            printf("\n========== MENU DO CLIENTE ==========\n");
                            printf("Cliente: %s\n", clienteEncontrado->nome);
                            printf("Conta: %s\n", contaEncontrada->numeroConta);

                            printf("Saldo: R$ %s%ld,%02ld\n",
                                contaEncontrada->saldo < 0 ? "-" : "",
                                labs(contaEncontrada->saldo / 100),
                                labs(contaEncontrada->saldo % 100));

                            printf("\n1 - Depositar\n");
                            printf("2 - Sacar\n");
                            printf("3 - Solicitar emprestimo\n");
                            printf("4 - Adquirir titulos de capitalizacao\n");
                            printf("5 - Solicitar extrato\n");
                            printf("0 - Voltar ao menu principal\n");
                            printf("s - Sair do programa\n");
                            printf("Escolha uma opcao: ");

                            scanf(" %c", &opcaoCliente);

                            switch (opcaoCliente) {

                                case '1': {

                                    long valor;

                                    printf("\nDigite o valor do deposito em centavos: ");
                                    scanf("%ld", &valor);

                                    depositar(contaEncontrada, valor);

                                    break;
                                }

                                case '2': {

                                    long valor;

                                    printf("\nDigite o valor do saque em centavos: ");
                                    scanf("%ld", &valor);

                                    sacar(contaEncontrada, valor);

                                    break;
                                }

                                case '3':

                                    solicitarEmprestimo(clienteEncontrado, contaEncontrada);

                                    break;

                                case '4':

                                    comprarTitulo(clienteEncontrado);

                                    break;

                                case '5':

                                    exibirExtrato(contaEncontrada);

                                    break;

                                case '0':

                                    printf("\nRetornando ao menu principal...\n");

                                    break;

                                case 's':
                                case 'S':

                                    sair = 1;

                                    break;

                                default:

                                    printf("\nOpcao invalida!\n");

                            }

                        } while (opcaoCliente != '0' && sair == 0);

                    } else {

                        printf("\nConta nao encontrada!\n");

                    }

                } else {

                    printf("\nAbra o banco primeiro!\n");

                }

                break;

            case 's':
            case 'S':

                sair = 1;

                break;

            default:

                printf("\nOpcao invalida!\n");

        }

    } while (sair == 0);

    if (clientes != NULL) {

        salvarDados(clientes, quantidadeClientes);

        free(clientes);

    }

    printf("\nBanco encerrado. Ate logo!\n");

    return 0;
}
