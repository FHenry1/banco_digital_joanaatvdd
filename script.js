const INTERVALO_MS = 3000;

const SECOES = [
    { chave: 'aptos', titulo: 'Clientes aptos a empréstimos', coluna: 'Valor concedido' },
    { chave: 'negativos', titulo: 'Clientes com saldo negativo', coluna: 'Saldo' },
    { chave: 'titulos', titulo: 'Clientes aptos à compra de títulos', coluna: 'Saldo' },
    { chave: 'contemplados', titulo: 'Clientes contemplados no sorteio', coluna: 'Prêmio' }
];

const formatadorMoeda = new Intl.NumberFormat('pt-BR', { style: 'currency', currency: 'BRL' });

function formatarValor(centavos) {
    return formatadorMoeda.format(centavos / 100);
}

function criarElemento(tag, classe, texto) {
    const elemento = document.createElement(tag);

    if (classe) {
        elemento.className = classe;
    }

    if (texto !== undefined) {
        elemento.textContent = texto;
    }

    return elemento;
}

function criarSecao(secao) {
    const cartao = criarElemento('section', 'cartao');
    const tabela = criarElemento('table');

    tabela.id = 'tabela-' + secao.chave;
    cartao.appendChild(criarElemento('h2', null, secao.titulo));
    cartao.appendChild(tabela);

    return cartao;
}

function preencherTabela(tabela, itens, coluna) {
    tabela.replaceChildren();

    if (itens.length === 0) {
        const celulaVazia = tabela.insertRow().insertCell();

        celulaVazia.className = 'vazio';
        celulaVazia.textContent = 'Nenhum cliente encontrado.';
        return;
    }

    const cabecalho = tabela.createTHead().insertRow();

    ['Cliente', 'Conta', coluna].forEach(function (titulo) {
        cabecalho.appendChild(criarElemento('th', null, titulo));
    });

    const corpo = tabela.createTBody();

    itens.forEach(function (item) {
        const linha = corpo.insertRow();
        const celulaValor = document.createElement('td');

        linha.insertCell().textContent = item.nome;
        linha.insertCell().textContent = item.numero;

        celulaValor.className = 'numero';
        celulaValor.textContent = formatarValor(item.valor);
        linha.appendChild(celulaValor);
    });
}

function renderizar(dados) {
    document.getElementById('saldo-total').textContent = formatarValor(dados.saldoTotal);
    document.getElementById('quantidade').textContent = dados.quantidadeClientes + ' clientes';

    SECOES.forEach(function (secao) {
        preencherTabela(
            document.getElementById('tabela-' + secao.chave),
            dados[secao.chave],
            secao.coluna
        );
    });
}

function definirStatus(mensagem, comErro) {
    const status = document.getElementById('status');

    status.textContent = mensagem;
    status.classList.toggle('erro', Boolean(comErro));
}

function carregarDados() {
    return new Promise(function (resolver, rejeitar) {
        const script = document.createElement('script');

        script.src = 'dados.js?t=' + Date.now();

        script.onload = function () {
            script.remove();
            resolver(window.DADOS_BANCO);
        };

        script.onerror = function () {
            script.remove();
            rejeitar(new Error('dados.js não encontrado'));
        };

        document.head.appendChild(script);
    });
}

async function atualizar() {
    try {
        renderizar(await carregarDados());
        definirStatus('Atualizado às ' + new Date().toLocaleTimeString('pt-BR'), false);
    } catch (erro) {
        definirStatus('Não encontrei o dados.js. Abra o banco no programa em C (opção 1).', true);
    }
}

SECOES.forEach(function (secao) {
    document.getElementById('painel').appendChild(criarSecao(secao));
});

document.getElementById('atualizar').addEventListener('click', atualizar);

atualizar();
setInterval(atualizar, INTERVALO_MS);
