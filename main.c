
#include <stdio.h>
#include <stdlib.h>
#include "meu_banco.h"

int main() {

    Cliente *clientes = NULL;

    int quantidadeClientes = 0;
    int proximoNumeroConta = 1000;

    int bancoAberto = 0;
    int sair = 0;

    char opcao;
    char numeroConta[TAM_NUMERO_CONTA];

    Cliente *clienteEncontrado;
    Conta *contaEncontrada;
    
    do {

        printf("\n== BANCO DIGITAL ==\n");
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

                    bancoAberto = abrirBanco(
                        &clientes,
                        &quantidadeClientes,
                        &proximoNumeroConta
                    );

                } else {

                    printf("\nO banco ja esta aberto!\n");

                }

                break;

            case '2':

                if (bancoAberto == 1) {

                    consultarHistorico(clientes, quantidadeClientes);

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
                            printf("Conta: %s\n", contaEncontrada->numero);
                            printf("Saldo: R$ %.2f\n", contaEncontrada->saldo);

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

                                case '1':

                                    depositar(contaEncontrada);

                                    break;

                                case '2':

                                    sacar(contaEncontrada);

                                    break;

                                case '3':

                                    solicitarEmprestimo(clienteEncontrado);

                                    break;

                                case '4':

                                    adquirirTitulo(clienteEncontrado);

                                    break;

                                case '5':

                                    consultarExtrato(contaEncontrada);

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

                printf("\nOpcao invalida! Tente novamente.\n");

                break;
        }

    } while (sair == 0);
    liberarBanco(clientes, quantidadeClientes);

    printf("\nSistema encerrado. Ate logo!\n");

    return 0;
}
