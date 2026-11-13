
#include <stdio.h>
#include <stdlib.h>
#include "meu_banco.h"

void gerarNumeroConta(Conta *conta, int numero, int tipo) {
    conta->numero = numero;
    conta->tipo = tipo;

    snprintf(conta->numeroConta, sizeof(conta->numeroConta),
             "%04d-%02d", numero, tipo);
}

void criarContas(Cliente *cliente, int numeroCliente) {
    int opcao;

    printf("\nEscolha o tipo de conta:\n");
    printf("1 - Corrente + Poupanca\n");
    printf("2 - Salario + Poupanca\n");
    printf("3 - Somente Salario\n");
    printf("4 - Somente Corrente\n");
    printf("5 - Somente Poupanca\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    while (opcao < 1 || opcao > 5) {
        printf("Opcao invalida! Escolha novamente: ");
        scanf("%d", &opcao);
    }

    cliente->quantidadeContas = 0;

    switch (opcao) {
        case 1:
            cliente->quantidadeContas = 2;
            cliente->contas[0].tipo = CORRENTE;
            cliente->contas[1].tipo = POUPANCA;
            break;

        case 2:
            cliente->quantidadeContas = 2;
            cliente->contas[0].tipo = SALARIO;
            cliente->contas[1].tipo = POUPANCA;
            break;

        case 3:
            cliente->quantidadeContas = 1;
            cliente->contas[0].tipo = SALARIO;
            break;

        case 4:
            cliente->quantidadeContas = 1;
            cliente->contas[0].tipo = CORRENTE;
            break;

        case 5:
            cliente->quantidadeContas = 1;
            cliente->contas[0].tipo = POUPANCA;
            break;
    }

    for (int i = 0; i < cliente->quantidadeContas; i++) {
        gerarNumeroConta(&cliente->contas[i], numeroCliente,
                         cliente->contas[i].tipo);

        cliente->contas[i].saldo = (rand() % 490001) + 10000;
        cliente->contas[i].quantidadeTransacoes = 0;
    }
}

void cadastrarClientes(Cliente clientes[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n== CLIENTE %d ==\n", i + 1);

        clientes[i].codigo = i + 1;

        printf("Nome do cliente: ");
        scanf(" %49[^\n]", clientes[i].nome);

        criarContas(&clientes[i], i + 1);

        clientes[i].possuiEmprestimo = 0;

        printf("\nCliente cadastrado com sucesso!\n");

        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            Conta *conta = &clientes[i].contas[j];

            printf("Conta: %s | Saldo: R$ %ld,%02ld\n",
                   conta->numeroConta,
                   conta->saldo / 100,
                   conta->saldo % 100);
        }
    }
}

void registrarTransacao(Conta *conta, long valor, int tipo) {
    if (conta->quantidadeTransacoes == MAX_TRANSACOES) {
        for (int i = 0; i < MAX_TRANSACOES - 1; i++) {
            conta->extrato[i] = conta->extrato[i + 1];
        }

        conta->quantidadeTransacoes--;
    }

    int posicao = conta->quantidadeTransacoes;

    conta->extrato[posicao].valor = valor;
    conta->extrato[posicao].tipoTransacao = tipo;

    conta->quantidadeTransacoes++;
}

void depositar(Conta *conta, long valor) {
    if (valor <= 0) {
        printf("Valor de deposito invalido!\n");
        return;
    }

    conta->saldo += valor;
    registrarTransacao(conta, valor, DEPOSITO);

    printf("Deposito realizado com sucesso!\n");
}

void sacar(Conta *conta, long valor) {
    if (valor <= 0) {
        printf("Valor de saque invalido!\n");
        return;
    }

    if (conta->tipo != CORRENTE && valor > conta->saldo) {
        printf("Saldo insuficiente!\n");
        return;
    }

    conta->saldo -= valor;
    registrarTransacao(conta, valor, SAQUE);

    printf("Saque realizado com sucesso!\n");
}

long calcularSaldoTotal(Cliente clientes[], int quantidade) {
    long saldoTotal = 0;

    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            saldoTotal += clientes[i].contas[j].saldo;
        }
    }

    return saldoTotal;
}

int clienteAptoEmprestimo(Cliente cliente) {
    int temCorrente = 0;
    int temPoupanca = 0;
    long saldoPoupanca = 0;

    for (int i = 0; i < cliente.quantidadeContas; i++) {
        if (cliente.contas[i].tipo == CORRENTE) {
            temCorrente = 1;
        }

        if (cliente.contas[i].tipo == POUPANCA) {
            temPoupanca = 1;
            saldoPoupanca = cliente.contas[i].saldo;
        }
    }

    return temCorrente && temPoupanca &&
           saldoPoupanca > 0 && !cliente.possuiEmprestimo;
}

long calcularLimite(Cliente cliente) {
    long saldoTotal = 0;

    for (int i = 0; i < cliente.quantidadeContas; i++) {
        saldoTotal += cliente.contas[i].saldo;
    }

    return (saldoTotal * 5) / 2;
}

void exibirSaldoTotal(Cliente clientes[], int quantidade) {
    long total = calcularSaldoTotal(clientes, quantidade);

    printf("\n== SALDO TOTAL DO BANCO ==\n");

    if (total < 0) {
        printf("Saldo total do banco: -R$ %ld,%02ld\n",
               -total / 100, -total % 100);
    } else {
        printf("Saldo total do banco: R$ %ld,%02ld\n",
               total / 100, total % 100);
    }
}

void exibirClientesAptos(Cliente clientes[], int quantidade) {
    int encontrou = 0;

    printf("\n== CLIENTES APTOS A EMPRESTIMO ==\n");

    for (int i = 0; i < quantidade; i++) {
        if (clienteAptoEmprestimo(clientes[i])) {
            long limite = calcularLimite(clientes[i]);

            if (limite <= 0) {
                continue;
            }

            encontrou = 1;

            printf("\nCliente: %s\n", clientes[i].nome);

            for (int j = 0; j < clientes[i].quantidadeContas; j++) {
                if (clientes[i].contas[j].tipo == CORRENTE) {
                    printf("Numero da conta: %s\n",
                           clientes[i].contas[j].numeroConta);
                    break;
                }
            }

            printf("Valor maximo: R$ %ld,%02ld\n",
                   limite / 100, limite % 100);
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente apto encontrado.\n");
    }
}

void exibirClientesNegativos(Cliente clientes[], int quantidade) {
    int encontrou = 0;

    printf("\n== CLIENTES NEGATIVOS ==\n");

    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            Conta *conta = &clientes[i].contas[j];

            if (conta->tipo == CORRENTE && conta->saldo < 0) {
                encontrou = 1;

                printf("\nCliente: %s\n", clientes[i].nome);
                printf("Numero da conta: %s\n", conta->numeroConta);
                printf("Saldo negativo: -R$ %ld,%02ld\n",
                       -conta->saldo / 100,
                       -conta->saldo % 100);
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente negativo encontrado.\n");
    }
}

void exibirClientesTitulos(Cliente clientes[], int quantidade) {
    int encontrou = 0;

    printf("\n==CLIENTES TITULOS ==\n");

    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            Conta *conta = &clientes[i].contas[j];

            if (conta->tipo == SALARIO &&
                conta->saldo >= 10000 &&
                conta->saldo <= 150000) {

                encontrou = 1;

                printf("\nCliente: %s\n", clientes[i].nome);
                printf("Numero da conta: %s\n", conta->numeroConta);
                printf("Saldo: R$ %ld,%02ld\n",
                       conta->saldo / 100,
                       conta->saldo % 100);
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente encontrado.\n");
    }
}

void exibirRelatorios(Cliente clientes[], int quantidade) {
    exibirSaldoTotal(clientes, quantidade);
    exibirClientesAptos(clientes, quantidade);
    exibirClientesNegativos(clientes, quantidade);
    exibirClientesTitulos(clientes, quantidade);
}
