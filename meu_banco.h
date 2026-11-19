#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#define MAX_CONTAS 2
#define MAX_TRANSACOES 5
#define TAM_NUMERO_CONTA 16

#define NUMERO_INICIAL 1000

#define CORRENTE 1
#define SALARIO 2
#define POUPANCA 3

#define DEPOSITO 1
#define SAQUE 2
#define EMPRESTIMO 3

#define TITULO_SALDO_MIN 100000
#define TITULO_SALDO_MAX 150000

typedef struct {
    long valor;
    int tipoTransacao;
} Transacao;

typedef struct {
    int numero;
    int tipo;
    long saldo;
    char numeroConta[TAM_NUMERO_CONTA];

    Transacao extrato[MAX_TRANSACOES];
    int quantidadeTransacoes;
} Conta;

typedef struct {
    int codigo;
    char nome[100];

    Conta contas[MAX_CONTAS];
    int quantidadeContas;

    int possuiEmprestimo;
    int possuiTitulo;
    long premioSorteio;
} Cliente;

void cadastrarClientes(Cliente clientes[], int quantidade);
void criarContas(Cliente *cliente, int numeroCliente);
void gerarNumeroConta(Conta *conta, int numero, int tipo);

int buscarConta(
    Cliente clientes[],
    int quantidade,
    char numeroConta[],
    Cliente **clienteEncontrado,
    Conta **contaEncontrada
);

void registrarTransacao(Conta *conta, long valor, int tipo);
void depositar(Conta *conta, long valor);
void sacar(Conta *conta, long valor);
void exibirExtrato(Conta *conta);
void alertarSaldoNegativo(Cliente *cliente);

int clienteAptoEmprestimo(Cliente cliente);
long calcularLimite(Cliente cliente);
void solicitarEmprestimo(Cliente *cliente);
void comprarTitulo(Cliente *cliente);

long calcularSaldoTotal(Cliente clientes[], int quantidade);
void exibirSaldoTotal(Cliente clientes[], int quantidade);
void exibirClientesAptos(Cliente clientes[], int quantidade);
void exibirClientesNegativos(Cliente clientes[], int quantidade);
void exibirClientesTitulos(Cliente clientes[], int quantidade);
void exibirClientesContemplados(
    Cliente *cliente,
    Conta *conta,
    long premio
);
void exibirRelatorios(Cliente clientes[], int quantidade);

int sorteioFibonacci(int quantidade);
void realizarSorteio(Cliente clientes[], int quantidade);

void salvarDados(Cliente clientes[], int quantidade);
int carregarDados(Cliente **clientes, int *quantidade);

#endif
