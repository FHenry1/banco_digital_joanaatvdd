#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#define MAX_CONTAS 2
#define MAX_TRANSACOES 5

#define CORRENTE 1
#define SALARIO 2
#define POUPANCA 3

#define DEPOSITO 1
#define SAQUE 2
#define EMPRESTIMO 3

typedef struct {
    long valor;
    int tipoTransacao;
} Transacao;

typedef struct {
    int numero;
    int tipo;
    long saldo;
    char numeroConta[16];

    Transacao extrato[MAX_TRANSACOES];
    int quantidadeTransacoes;
} Conta;

typedef struct {
    int codigo;
    char nome[50];

    Conta contas[MAX_CONTAS];
    int quantidadeContas;

    int possuiEmprestimo;
} Cliente;

void cadastrarClientes(Cliente clientes[], int quantidade);
void criarContas(Cliente *cliente, int numeroCliente);
void gerarNumeroConta(Conta *conta, int numero, int tipo);

void depositar(Conta *conta, long valor);
void sacar(Conta *conta, long valor);


void registrarTransacao(Conta *conta, long valor, int tipo);


long calcularSaldoTotal(Cliente clientes[], int quantidade);
int clienteAptoEmprestimo(Cliente cliente);
long calcularLimite(Cliente cliente);


void exibirSaldoTotal(Cliente clientes[], int quantidade);
void exibirClientesAptos(Cliente clientes[], int quantidade);
void exibirClientesNegativos(Cliente clientes[], int quantidade);
void exibirClientesTitulos(Cliente clientes[], int quantidade);
void exibirRelatorios(Cliente clientes[], int quantidade);

#endif