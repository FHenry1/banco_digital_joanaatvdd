#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "meu_banco.h"

static int proximoNumeroConta = 1000;

void gerarNumeroConta(Conta *conta, int numero, int tipo) {
    conta->numero = numero;
    conta->tipo = tipo;

    snprintf(conta->numeroConta, sizeof(conta->numeroConta),
             "%04d-%02d", numero, tipo);
}

void criarContas(Cliente *cliente, int numeroCliente) {

    int opcao = (rand() % 5) + 1;
    int tipos[2];
    int quantidade = 0;

    switch (opcao) {
        case 1:
            tipos[0] = CORRENTE;
            tipos[1] = POUPANCA;
            quantidade = 2;
            break;

        case 2:
            tipos[0] = SALARIO;
            tipos[1] = POUPANCA;
            quantidade = 2;
            break;

        case 3:
            tipos[0] = SALARIO;
            quantidade = 1;
            break;

        case 4:
            tipos[0] = CORRENTE;
            quantidade = 1;
            break;

        case 5:
            tipos[0] = POUPANCA;
            quantidade = 1;
            break;
    }

    cliente->quantidadeContas = quantidade;
    
    for (int i = 0; i < quantidade; i++) {
        Conta *conta = &cliente->contas[i];

        memset(conta, 0, sizeof(Conta));

        gerarNumeroConta(conta, proximoNumeroConta++, tipos[i]);

        conta->saldo = 0;
        conta->quantidadeTransacoes = 0;
    }
}


void cadastrarClientes(Cliente clientes[], int quantidade) {

    for (int i = 0; i < quantidade; i++) {

        clientes[i].codigo = i + 1;

        printf("\n== CADASTRO DO CLIENTE %d ==\n", i + 1);
        printf("Digite o nome do cliente: ");

        scanf(" %99[^\n]", clientes[i].nome);

        criarContas(&clientes[i], i + 1);

        clientes[i].possuiEmprestimo = 0;
        clientes[i].possuiTitulo = 0;

        printf("\nCliente cadastrado com sucesso!\n");
        printf("Nome: %s\n", clientes[i].nome);

        for (int j = 0; j < clientes[i].quantidadeContas; j++) {

            printf("Conta: %s\n", clientes[i].contas[j].numeroConta);
        }
    }

    printf("\nTodos os clientes foram cadastrados!\n");
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

    if (conta->saldo > LONG_MAX - valor) {
        printf("Operacao excede o limite permitido!\n");
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

    if (conta->saldo < LONG_MIN + valor) {
        printf("Operacao excede o limite permitido!\n");
        return;
    }

    conta->saldo -= valor;

    registrarTransacao(conta, valor, SAQUE);

    printf("Saque realizado com sucesso!\n");

    if (conta->tipo == CORRENTE && conta->saldo < 0) {
        printf("\nALERTA: Sua conta corrente esta com saldo negativo!\n");
    }
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
    return temCorrente &&
           temPoupanca &&
           saldoPoupanca > 0 &&
           !cliente.possuiEmprestimo;
}


long calcularLimite(Cliente cliente) {
    long saldoTotal = 0;

    for (int i = 0; i < cliente.quantidadeContas; i++) {
        if (cliente.contas[i].tipo == POUPANCA) {
            saldoTotal = cliente.contas[i].saldo;
            break;
        }
    }

    if (saldoTotal <= 0) {
        return 0;
    }

    if (saldoTotal > LONG_MAX / 10) {
        return LONG_MAX;
    }

    long valorDezVezes = saldoTotal * 10;

    return valorDezVezes / 4;
}

void solicitarEmprestimo(Cliente *cliente) {
    if (!clienteAptoEmprestimo(*cliente)) {
        printf("Cliente nao esta apto a solicitar emprestimo.\n");
        return;
    }

    long limite = calcularLimite(*cliente);

    if (limite <= 0) {
        printf("Limite de emprestimo indisponivel.\n");
        return;
    }

    printf("\nLimite disponivel: R$ %ld,%02ld\n",
           limite / 100, limite % 100);

    long valor;

    printf("Digite o valor do emprestimo em centavos: ");
    if (scanf("%ld", &valor) != 1) {
        printf("Valor invalido!\n");
        return;
    }

    if (valor <= 0 || valor > limite) {
        printf("Valor de emprestimo nao permitido!\n");
        return;
    }

    for (int i = 0; i < cliente->quantidadeContas; i++) {
        Conta *conta = &cliente->contas[i];

        if (conta->tipo == CORRENTE) {
            if (conta->saldo > LONG_MAX - valor) {
                printf("Operacao excede o limite permitido!\n");
                return;
            }

            conta->saldo += valor;
            registrarTransacao(conta, valor, EMPRESTIMO);

            cliente->possuiEmprestimo = 1;

            printf("Emprestimo concedido com sucesso!\n");
            printf("Valor: R$ %ld,%02ld\n",
                   valor / 100, valor % 100);
            return;
        }
    }
}

void comprarTitulo(Cliente *cliente) {
    if (cliente->possuiTitulo) {
        printf("Este cliente ja possui um titulo de capitalizacao.\n");
        return;
    }

    for (int i = 0; i < cliente->quantidadeContas; i++) {
        Conta *conta = &cliente->contas[i];

        if (conta->tipo == SALARIO) {
            if (conta->saldo >= 100000 && conta->saldo <= 150000) {
                cliente->possuiTitulo = 1;

                printf("Titulo de capitalizacao adquirido com sucesso!\n");
                return;
            }

            printf("Saldo da conta salario fora do limite permitido.\n");
            printf("Necessario possuir entre R$ 1.000,00 e R$ 1.500,00.\n");
            return;
        }
    }

    printf("Cliente nao possui conta salario.\n");
}

void exibirExtrato(Conta *conta) {
    printf("\n== EXTRATO DA CONTA %s ==\n", conta->numeroConta);

    if (conta->quantidadeTransacoes == 0) {
        printf("Nenhuma transacao realizada.\n");
        return;
    }

    int inicio = conta->quantidadeTransacoes - 5;

    if (inicio < 0) {
        inicio = 0;
    }

    for (int i = inicio; i < conta->quantidadeTransacoes; i++) {
        char operacao;

        switch (conta->extrato[i].tipoTransacao) {
            case SAQUE:
                operacao = 'S';
                break;

            case DEPOSITO:
                operacao = 'D';
                break;

            case EMPRESTIMO:
                operacao = 'E';
                break;

            default:
                operacao = '?';
                break;
        }

        printf("R$ %ld,%02ld : %c\n",
               conta->extrato[i].valor / 100,
               conta->extrato[i].valor % 100,
               operacao);
    }
}

int buscarConta(Cliente clientes[], int quantidade,
                char numeroConta[], Cliente **clienteEncontrado,
                Conta **contaEncontrada) {
    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            if (strcmp(clientes[i].contas[j].numeroConta, numeroConta) == 0) {
                *clienteEncontrado = &clientes[i];
                *contaEncontrada = &clientes[i].contas[j];
                return 1;
            }
        }
    }

    return 0;
}

void alertarSaldoNegativo(Cliente *cliente) {
    for (int i = 0; i < cliente->quantidadeContas; i++) {
        Conta *conta = &cliente->contas[i];

        if (conta->tipo == CORRENTE && conta->saldo < 0) {
            printf("\nATENCAO: Sua conta corrente %s esta negativa!\n",
                   conta->numeroConta);

            printf("Saldo atual: -R$ %ld,%02ld\n",
                   -conta->saldo / 100,
                   -conta->saldo % 100);
        }
    }
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

void exibirSaldoTotal(Cliente clientes[], int quantidade) {
    long total = calcularSaldoTotal(clientes, quantidade);

    printf("\n== SALDO TOTAL DO BANCO ==\n");
    printf("%d clientes | ", quantidade);

    if (total < 0) {
        printf("-R$ %ld,%02ld\n", -total / 100, -total % 100);
    } else {
        printf("R$ %ld,%02ld\n", total / 100, total % 100);
    }
}

void exibirClientesAptos(Cliente clientes[], int quantidade) {
    int encontrou = 0;

    printf("\n== CLIENTES APTOS A EMPRESTIMOS ==\n");

    for (int i = 0; i < quantidade; i++) {
        if (clienteAptoEmprestimo(clientes[i])) {
            long limite = calcularLimite(clientes[i]);

            if (limite <= 0) {
                continue;
            }

            encontrou = 1;

            for (int j = 0; j < clientes[i].quantidadeContas; j++) {
                if (clientes[i].contas[j].tipo == CORRENTE) {
                    printf("%s | %s | R$ %ld,%02ld\n",
                           clientes[i].nome,
                           clientes[i].contas[j].numeroConta,
                           limite / 100,
                           limite % 100);
                    break;
                }
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente apto encontrado.\n");
    }
}

void exibirClientesNegativos(Cliente clientes[], int quantidade) {
    int encontrou = 0;

    printf("\n== CLIENTES COM SALDO NEGATIVO NA CONTA CORRENTE ==\n");

    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            Conta *conta = &clientes[i].contas[j];

            if (conta->tipo == CORRENTE && conta->saldo < 0) {
                encontrou = 1;

                printf("%s | %s | -R$ %ld,%02ld\n",
                       clientes[i].nome,
                       conta->numeroConta,
                       -conta->saldo / 100,
                       -conta->saldo % 100);
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente com saldo negativo encontrado.\n");
    }
}

void exibirClientesTitulos(Cliente clientes[], int quantidade) {
    int encontrou = 0;

    printf("\n== CLIENTES APTOS A COMPRA DE TITULOS ==\n");

    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            Conta *conta = &clientes[i].contas[j];

            if (conta->tipo == SALARIO &&
                conta->saldo >= 100000 &&
                conta->saldo <= 150000) {
                encontrou = 1;

                printf("%s | %s | R$ %ld,%02ld\n",
                       clientes[i].nome,
                       conta->numeroConta,
                       conta->saldo / 100,
                       conta->saldo % 100);
                break;
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente apto encontrado.\n");
    }
}

void exibirClientesContemplados(Cliente *cliente, Conta *conta, long premio) {
    printf("\n== CLIENTE CONTEMPLADO NO SORTEIO ==\n");
    printf("%s | %s | R$ %ld,%02ld\n",
           cliente->nome,
           conta->numeroConta,
           premio / 100,
           premio % 100);
}

void exibirRelatorios(Cliente clientes[], int quantidade) {
    exibirSaldoTotal(clientes, quantidade);
    exibirClientesAptos(clientes, quantidade);
    exibirClientesNegativos(clientes, quantidade);
    exibirClientesTitulos(clientes, quantidade);
}

int sorteioFibonacci(int quantidade) {
    if (quantidade <= 0) {
        return -1;
    }

    int posicao = 10 + rand() % 90;
    long anterior = 0;
    long atual = 1;

    for (int i = 0; i < posicao; i++) {
        long proximo = (anterior + atual) % quantidade;
        anterior = atual;
        atual = proximo;
    }

    return (int)(anterior % quantidade);
}

void realizarSorteio(Cliente clientes[], int quantidade) {
    if (quantidade < 5) {
        printf("Sao necessarios pelo menos cinco clientes.\n");
        return;
    }

    int melhores[5];
    long saldos[5];

    for (int i = 0; i < 5; i++) {
        melhores[i] = -1;
        saldos[i] = LONG_MIN;
    }

    for (int i = 0; i < quantidade; i++) {
        long saldo = 0;

        for (int j = 0; j < clientes[i].quantidadeContas; j++) {
            saldo += clientes[i].contas[j].saldo;
        }

        for (int j = 0; j < 5; j++) {
            if (saldo > saldos[j]) {
                for (int k = 4; k > j; k--) {
                    saldos[k] = saldos[k - 1];
                    melhores[k] = melhores[k - 1];
                }

                saldos[j] = saldo;
                melhores[j] = i;
                break;
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (melhores[i] == -1 ||
            !clientes[melhores[i]].possuiTitulo) {
            printf("Sorteio nao realizado.\n");
            printf("Todos os cinco maiores saldos precisam possuir titulos.\n");
            return;
        }
    }

    int vencedor = sorteioFibonacci(5);
    int indiceCliente = melhores[vencedor];

    Cliente *cliente = &clientes[indiceCliente];
    Conta *contaPremiada = NULL;

    for (int i = 0; i < cliente->quantidadeContas; i++) {
        if (contaPremiada == NULL ||
            cliente->contas[i].saldo > contaPremiada->saldo) {
            contaPremiada = &cliente->contas[i];
        }
    }

    if (contaPremiada == NULL) {
        printf("Nao foi possivel realizar o sorteio.\n");
        return;
    }

    if (contaPremiada->saldo <= 0) {
        printf("O vencedor nao possui saldo positivo para calcular o premio.\n");
        return;
    }

    if (contaPremiada->saldo > LONG_MAX / 10) {
        printf("Valor do premio excede o limite permitido.\n");
        return;
    }

    long premio = contaPremiada->saldo * 10;

    if (contaPremiada->saldo > LONG_MAX - premio) {
        printf("Nao foi possivel creditar o premio.\n");
        return;
    }

    contaPremiada->saldo += premio;

    registrarTransacao(contaPremiada, premio, DEPOSITO);

    printf("\nSORTEIO REALIZADO COM SUCESSO!\n");

    exibirClientesContemplados(cliente, contaPremiada, premio);
}

void salvarDados(Cliente clientes[], int quantidade) {
    FILE *arquivo = fopen("banco.dat", "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar os dados do banco.\n");
        return;
    }

    fwrite(&quantidade, sizeof(int), 1, arquivo);
    fwrite(clientes, sizeof(Cliente), quantidade, arquivo);

    fclose(arquivo);

    printf("Dados salvos com sucesso!\n");
}

int carregarDados(Cliente **clientes, int *quantidade) {
    FILE *arquivo = fopen("banco.dat", "rb");

    if (arquivo == NULL) {
        return 0;
    }

    int total;

    if (fread(&total, sizeof(int), 1, arquivo) != 1 ||
        total <= 0 || total > 1000000) {
        fclose(arquivo);
        return 0;
    }

    Cliente *dados = malloc(total * sizeof(Cliente));

    if (dados == NULL) {
        fclose(arquivo);
        return 0;
    }

    if (fread(dados, sizeof(Cliente), total, arquivo) != (size_t)total) {
        free(dados);
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    *clientes = dados;
    *quantidade = total;

    proximoNumeroConta = 1000;

    for (int i = 0; i < total; i++) {
        for (int j = 0; j < dados[i].quantidadeContas; j++) {
            if (dados[i].contas[j].numero >= proximoNumeroConta) {
                proximoNumeroConta = dados[i].contas[j].numero + 1;
            }
        }
    }

    return 1;
}
